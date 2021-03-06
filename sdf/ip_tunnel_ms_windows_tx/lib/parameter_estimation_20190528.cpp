#include "..\include\parameter_estimation_20190528.h"

void ParameterEstimation::initialize(void) {

	// Input: Key

	// Input: Seed and Bits

	// Output: Key
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	// Output: Seed and Bits

	// Other Outputs: BER

	bitsFromAlice.reserve(numberOfBits);
	bitsFromBob.reserve(numberOfBits);
	inputKey.reserve(2 * numberOfBitsRequiredToStart);
	outputKey.reserve(numberOfBitsRequiredToStart);
	msgInput.reserve(2 * numberOfBitsRequiredToStart);
	msgOutput.reserve(numberOfBitsRequiredToStart);

	firstPass = false;
	double x1 = -2;
	double x2 = 2;
	double x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha) * (x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
	double exacteness = 1e-15;

	while (abs(erf(x3 / sqrt(2)) + 1 - alpha) > exacteness)
	{
		x3 = x2 - (erf(x2 / sqrt(2)) + 1 - alpha) * (x2 - x1) / (erf(x2 / sqrt(2)) - erf(x1 / sqrt(2)));
		x1 = x2;
		x2 = x3;
	}

	z = -x3;

	if (!experimental::filesystem::is_directory(outputFolderName) || !experimental::filesystem::exists(outputFolderName)) {
		experimental::filesystem::create_directories(outputFolderName);
	}
	ofstream myfile;
	myfile.open(outputFolderName + bersListFileName);
	myfile << "BER \t\t Upper Bound \t\t Lower Bound\n";

	myfile.close();

}

bool ParameterEstimation::runBlock(void) {
	bool alive = false;


	int process;
	int ready = inputSignals[0]->ready();
	int spaceKey = outputSignals[0]->space();

	for (auto k = 0; k < ready; ++k) {
		t_binary dataIn;
		inputSignals[0]->bufferGet(&dataIn);
		inputKey.push_back(dataIn);
		totalNumberOfInputBits++;
	}
	if (!bypassParameterEstimation) {
		process = min(spaceKey, (int)outputKey.size());
		if (process > 0) {
			reportUpdated = false;
			for (auto k = 0; k < process; ++k) {
				outputSignals[0]->bufferPut((t_binary)outputKey[k]);
				totalNumberOfOutputBits++;
			}
			reportUpdated = false;
			outputKey.erase(outputKey.begin(), outputKey.begin() + process);
			alive = true;
		}
		if (parameterEstimationRole == parameter_estimation_role::Alice) {

			if (msgOutput.size() > 0) {
				finishedSendingMsg = MessageProcessors::sendToMsgProc(msgOutput, *outputSignals[1], sendingMsg);
				alive = true;
				if (!finishedSendingMsg) return alive;
			}

			int bitsReady = inputSignals[1]->ready();

			if (bitsReady > 0) {
				finishedReceivingMsg = MessageProcessors::recvFromMsgProc(msgInput, *inputSignals[1], startedReceivingMsg, numberOfValuesToReceive);
				alive = true;
				if (!finishedReceivingMsg) return alive;
			}

			if ((bitsReady == 0) && (msgOutput.empty())) {
				return alive;
			}

			int berSpace = 100;
			for (unsigned int  m = 2; m < outputSignals.size(); ++m) {
				berSpace = min(berSpace, outputSignals[m]->space());
			}
			if (berSpace < 2) return alive;

//			t_real seedReal;
//			inputSignals[1]->bufferGet(&seedReal);

			t_integer seed = msgInput[0];


/*			for (auto k = 1; k < msgInput.size(); ++k)
			{
				t_real bitsIn;
				inputSignals[1]->bufferGet(&bitsIn);
				bitsFromBob.push_back((int)bitsIn);
			} */

			selectBitsToUse(seed, numberOfBits, inputKey, bitsFromAlice, numberOfBitsRequiredToStart);

			coincidences = 0;

			for (auto k = 0; k < (int64_t)bitsFromAlice.size(); ++k) {
				if (msgInput[(int64_t)k + (int64_t)1] == bitsFromAlice[k]) {
					coincidences++;
					msgOutput.push_back((t_integer)1);
				}
				else {
					msgOutput.push_back((t_integer)0);
				}

			}

			BER = 1 - (coincidences / (t_real)numberOfBits);
			BERUpperBound = BER + 1.0 / sqrt((t_real)numberOfBits) * z * sqrt(BER * (1.0 - BER)) + 1.0 / (3.0 * (t_real)numberOfBits) * (2.0 * z * z * (1.0 / 2.0 - BER) + (2.0 - BER));
			BERLowerBound = BER - 1.0 / sqrt((t_real)numberOfBits) * z * sqrt(BER * (1.0 - BER)) + 1.0 / (3.0 * (t_real)numberOfBits) * (2.0 * z * z * (1.0 / 2.0 - BER) - (1.0 + BER));
			//			BERLowerBound = max(BERLowerBound, (t_real)0);
			berListUpdated = false;
			bool test = BERUpperBound > threshold;

			if (!test) {
				for (unsigned int k = 0; k < numberOfBitsRequiredToStart - numberOfBits; ++k) {
					outputKey.push_back(inputKey[k]);
				}
				for (unsigned int  m = 2; m < outputSignals.size(); ++m) {
					outputSignals[m]->bufferPut((t_real)BERUpperBound);
					outputSignals[m]->bufferPut((t_real)(numberOfBitsRequiredToStart - numberOfBits));
				}
			}

			finishedSendingMsg = MessageProcessors::sendToMsgProc(msgOutput, *outputSignals[1], sendingMsg);

			updateReportResults();

			inputKey.erase(inputKey.begin(), inputKey.begin() + ((int)numberOfBitsRequiredToStart - (int)numberOfBits));
			bitsFromAlice.erase(bitsFromAlice.begin(), bitsFromAlice.end());
			bitsFromBob.erase(bitsFromBob.begin(), bitsFromBob.end());
			msgInput.erase(msgInput.begin(), msgInput.end());

			alive = true;
		}
		else if (parameterEstimationRole == parameter_estimation_role::Bob) {
			
			if (msgOutput.size() > 0) {
				finishedSendingMsg = MessageProcessors::sendToMsgProc(msgOutput, *outputSignals[1], sendingMsg);
				if (!finishedSendingMsg) return true;
			}

			if (waitingForComm) {

				int bitsReady = inputSignals[1]->ready();

				if (bitsReady > 0) {
					finishedReceivingMsg = MessageProcessors::recvFromMsgProc(msgInput, *inputSignals[1], startedReceivingMsg, numberOfValuesToReceive);
					alive = true;
					if (!finishedReceivingMsg) return alive;
				}

				if ((bitsReady == 0) && (msgOutput.empty())) {
					return alive;
				}

				int berSpace = 100;
				for (unsigned int  m = 2; m < outputSignals.size(); ++m) {
					berSpace = min(berSpace, outputSignals[m]->space());
				}
				if (berSpace < 2) return alive;

				coincidences = 0;

				for (unsigned int  k = 0; k < msgInput.size(); ++k)
				{
					
					coincidences += msgInput[k];
				}

				BER = 1.0 - (coincidences / (t_real)numberOfBits);
				BERUpperBound = BER + 1.0 / sqrt((t_real)numberOfBits) * z * sqrt(BER * (1.0 - BER)) + 1.0 / (3.0 * (t_real)numberOfBits) * (2.0 * z * z * (1.0 / 2.0 - BER) + (2.0 - BER));
				BERLowerBound = BER - 1.0 / sqrt((t_real)numberOfBits) * z * sqrt(BER * (1.0 - BER)) + 1.0 / (3.0 * (t_real)numberOfBits) * (2.0 * z * z * (1.0 / 2.0 - BER) - (1.0 + BER));
				BERLowerBound = max(BERLowerBound, (t_real)0);
				berListUpdated = false;
				bool test = BERUpperBound > threshold;

				if (!test) {
					for (unsigned int k = 0; k < numberOfBitsRequiredToStart - numberOfBits; ++k) {
						outputKey.push_back(inputKey[k]);
					}

					for (unsigned int  m = 2; m < outputSignals.size(); ++m) {
						outputSignals[m]->bufferPut((t_real)BERUpperBound);
						outputSignals[m]->bufferPut((t_real)(numberOfBitsRequiredToStart - numberOfBits));
					}
					cout << "Parameter estimation - BER: " << BER << " - Upper bound: " << BERUpperBound << "\n";
				}

				updateReportResults();

				inputKey.erase(inputKey.begin(), inputKey.begin() + ((int)numberOfBitsRequiredToStart - (int)numberOfBits));
				msgInput.erase(msgInput.begin(), msgInput.end());

				waitingForComm = false;
			}
			else {
				if ((int64_t)inputKey.size() < numberOfBitsRequiredToStart) return alive;

				std::random_device rd;
				t_integer seed = rd();
				seed = abs(seed);
				msgOutput.push_back(seed);
				selectBitsToUse(seed, numberOfBits, inputKey, bitsToUse, numberOfBitsRequiredToStart);

				for (unsigned int  k = 0; k < bitsToUse.size(); ++k) {
					msgOutput.push_back(bitsToUse[k]);
				}
				bitsToUse.erase(bitsToUse.begin(), bitsToUse.end());

				finishedSendingMsg = MessageProcessors::sendToMsgProc(msgOutput, *outputSignals[1], sendingMsg);

				waitingForComm = true;
			}
			alive = true;
		}
		else {
			cout << "Undefined parameter estimation behavior.\n";
		}

		int outputProcess = min(spaceKey, (int)outputKey.size());
		for (auto k = 0; k < outputProcess; ++k) {
			outputSignals[0]->bufferPut((t_binary)outputKey[k]);
			totalNumberOfOutputBits++;
		}
		alive = true;
		outputKey.erase(outputKey.begin(), outputKey.begin() + outputProcess);

	}
	else {
		process = min(spaceKey, (int)inputKey.size());
		if (process <= 0) return alive;
		for (auto k = 0; k < process; ++k) {
			outputSignals[0]->bufferPut((t_binary)inputKey[k]);
			totalNumberOfOutputBits++;
		}
		inputKey.erase(inputKey.begin(), inputKey.begin() + process);
		alive = true;
	}

	if ((!reportUpdated) || !alive) {
		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open(outputFolderName + reportOutputFileName);
		myfile << "Total Average BER=           " << averageBER << "\n";
		myfile << "Highest estimated BER=       " << highestBER << "\n";
		myfile << "Lowest estimated BER=        " << lowestBER << "\n";
		myfile << "Upper and lower confidence bounds for " << (1 - alpha) * 100 << "% confidence level \n";

		myfile << "Highest Upper Bound=         " << highestBERUpperBound << "\n";
		myfile << "Lowest Upper Bound=          " << lowestBERUpperBound << "\n";
		myfile << "Highest Lower Bound=         " << highestBERLowerBound << "\n";
		myfile << "Lowest Lower Bound=          " << lowestBERLowerBound << "\n";
		myfile << "Number of measurements=      " << numberOfMeasurements << "\n";
		myfile << "Number of received bits=     " << totalNumberOfInputBits << "\n";
		myfile << "Number of sent bits=         " << totalNumberOfOutputBits << "\n";

		myfile << "Bits in estimation interval= " << numberOfBitsRequiredToStart << "\n";
		myfile << "Bits used for estimation=    " << numberOfBits << "\n";

		myfile.close();
		reportUpdated = false;

	}
	if (!berListUpdated) {
		/* Outputting a .txt report*/
		ofstream myfile;
		myfile.open(outputFolderName + bersListFileName, std::ios_base::app);
		myfile << BER << "\t\t" << BERUpperBound << "\t\t" << BERLowerBound << "\n";

		myfile.close();
		reportUpdated = false;
	}

	return alive;
}

bool ParameterEstimation::selectBitsToUse(const t_integer seedVal, const int64_t nBits, vector <t_integer>& vectorInputKey, vector <t_integer>& vectorBitsToUse, int64_t intervalSize)
{
	bool alive = false;

	UniformRandomRealNumbers<> uniformeRandomNumberBetweenZeroAndOne{ seedVal, 0, 1 };
	int64_t interval = intervalSize;
	t_integer randIdx{ 0 };
	t_real randNum{ 0 };

	for (auto k = 0; k < nBits; ++k) {
		randNum = uniformeRandomNumberBetweenZeroAndOne() * ((t_real)interval - 1);
		randIdx = (int)round(randNum);
		vectorBitsToUse.push_back(vectorInputKey[randIdx]);
		vectorInputKey.erase(vectorInputKey.begin() + randIdx);
		interval--;
	}

	alive = true;
	return alive;

}

void ParameterEstimation::updateReportResults(void) {
	numberOfMeasurements++;

	averageBER = ((averageBER * ((int64_t)numberOfMeasurements - 1)) + BER) / numberOfMeasurements;
	highestBER = max(highestBER, BER);
	lowestBER = min(lowestBER, BER);

	highestBERUpperBound = max(highestBERUpperBound, BERUpperBound);
	lowestBERUpperBound = min(lowestBERUpperBound, max(BERUpperBound, (t_real)0));
	highestBERLowerBound = max(highestBERLowerBound, BERLowerBound);
	lowestBERLowerBound = min(lowestBERLowerBound, max(BERLowerBound, (t_real)0));

	reportUpdated = false;
}
/*
bool ParameterEstimation::sendToMsg(vector <t_integer>& data, Signal& signalToMsg, bool& started) {
	int space = signalToMsg.space();
	if (space <= 0) return false;

	if (!started) signalToMsg.bufferPut((t_real)data.size());

	if (space <= 0) return false;
	started = true;

	int ready = (int)data.size();
	space = signalToMsg.space();
	int process = min(ready, space);

	for (auto k = 0; k < process; ++k) {
		signalToMsg.bufferPut((t_real)data[k]);
	}
	data.erase(data.begin(), data.begin() + process);

	if (data.empty()) {
		started = false;
		return true;
	}
	else {
		return false;
	}
}
*/