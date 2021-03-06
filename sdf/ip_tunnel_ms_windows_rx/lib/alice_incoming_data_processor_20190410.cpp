#include "..\include\alice_incoming_data_processor_20190410.h"

void AliceIncomingDataProcessor::initialize(void)
{
	outputSignals[0]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[0]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[0]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[3]->setSymbolPeriod(inputSignals[0]->getSymbolPeriod());
	outputSignals[3]->setSamplingPeriod(inputSignals[0]->getSamplingPeriod());
	outputSignals[3]->setFirstValueToBeSaved(inputSignals[0]->getFirstValueToBeSaved());

	outputSignals[2]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[2]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[2]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());


	outputSignals[1]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[1]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[1]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());

	outputSignals[4]->setSymbolPeriod(inputSignals[1]->getSymbolPeriod());
	outputSignals[4]->setSamplingPeriod(inputSignals[1]->getSamplingPeriod());
	outputSignals[4]->setFirstValueToBeSaved(inputSignals[1]->getFirstValueToBeSaved());

}

bool AliceIncomingDataProcessor::runBlock(void)
{
	bool alive{ false };
	t_binary dataIn;
	t_binary basisIn;

	int ready{ MAX_BUFFER_LENGTH };
	int space{ MAX_BUFFER_LENGTH };
//	int transmissionReady{ inputSignals[0]->ready() };

	for (unsigned int i = 1; i < inputSignals.size(); i++) {
		ready = (ready < inputSignals[i]->ready()) ? ready : inputSignals[i]->ready();
	}

//	for (auto k : inputSignals) { ready = (ready < k->ready()) ? ready : k->ready(); }
	for (auto k : outputSignals) { space = (space < k->space()) ? space : k->space(); }
	
	int process = min(ready, space);
	if (0 == process) return alive;
	
	/*
	for (unsigned int i = 0; i < process; i++) {
		inputSignals[0]->bufferGet(&transmissionStatus);
		inputSignals[1]->bufferGet(&dataIn);
		inputSignals[2]->bufferGet(&basisIn);

		if (1 == transmissionStatus) {
			outputSignals[0]->bufferPut(dataIn);
			outputSignals[2]->bufferPut(dataIn);
			outputSignals[1]->bufferPut(basisIn);
			outputSignals[3]->bufferPut(basisIn);
		}
	}*/

	for (auto k = 0; k < process; ++k) {
		inputSignals[0]->bufferGet(&dataIn);
		inputSignals[1]->bufferGet(&basisIn);
		t_binary transmissionMode = 0;
		if ((dataIn != 0) && (dataIn != 1)) {
			cout << "";
		}
		outputSignals[0]->bufferPut(dataIn);
		outputSignals[3]->bufferPut(dataIn);
		outputSignals[2]->bufferPut(transmissionMode);
		outputSignals[1]->bufferPut(basisIn);
		outputSignals[4]->bufferPut(basisIn);
	}
	
	alive = true;

	return alive;
}
