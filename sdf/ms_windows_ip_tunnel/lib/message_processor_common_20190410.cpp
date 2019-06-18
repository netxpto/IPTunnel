#include "../include/message_processor_common_20190410.h"

namespace MessageProcessors {

	t_message_type getMessageType(const t_message& msg) {
		if (msg.messageType[0] == t_message_type::BasisReconciliationMsg) return t_message_type::BasisReconciliationMsg;
		if (msg.messageType[0] == t_message_type::ErrorCorrectionMsg) return t_message_type::ErrorCorrectionMsg;
		if (msg.messageType[0] == t_message_type::ErrorCorrectionPermutationsMsg) return t_message_type::ErrorCorrectionPermutationsMsg;
		if (msg.messageType[0] == t_message_type::PrivacyAmplificationSeedsMsg) return t_message_type::PrivacyAmplificationSeedsMsg;
		if (msg.messageType[0] == t_message_type::ParameterEstimationSeedAndBitsMsg) return t_message_type::ParameterEstimationSeedAndBitsMsg;
		if (msg.messageType[0] == t_message_type::ToggleBERChangeMsg) return t_message_type::ToggleBERChangeMsg;
		return (t_message_type)0;
	}


	t_message_data getMessageData(const t_message & msg, t_message_data_length dataLength) {
		t_message_type mType = getMessageType(msg);
		string mDataString = msg.messageData;
		vector <int> mDataVector;
		string num{ "" };

		if (t_message_type::ErrorCorrectionPermutationsMsg == mType || t_message_type::PrivacyAmplificationSeedsMsg == mType) {
			for (int k = 0; k < dataLength; k++) {
				string data;
				data = mDataString.at(k);
				if (data == "#") {
					mDataVector.push_back(stoi(num));
					num = "";
				}
				else {
					num.append(data);
				}
			}
		}
		else if (t_message_type::ParameterEstimationSeedAndBitsMsg == mType) {
			bool gotSeed = false;

			for (int k = 0; k < dataLength; k++) {
				if (!gotSeed) {
					string data;
					data = mDataString.at(k);
					if (data == "#") {
						mDataVector.push_back(stoi(num));
						num = "";
						gotSeed = true;
					}
					else {
						num.append(data);
					}
				}
				else {
					char data = mDataString.at(k);
					if (data == '1') {
						mDataVector.push_back(1);
					}
					else if (data == '0') {
						mDataVector.push_back(0);
					}
				}
			}
		}
		else {
			for (int k = 0; k < dataLength; k++) {
				char data = mDataString.at(k);

				if (data == '1') {
					mDataVector.push_back(1);
				}
				else if (data == '0') {
					mDataVector.push_back(0);
				}
				else if (data == '2') {
					mDataVector.push_back(2);
				}
				else if (data == '3') {
					mDataVector.push_back(3);
				}
				else if (data == '4') {
					mDataVector.push_back(3);
				}
				else if (data == '5') {
					mDataVector.push_back(5);
				}
				else if (data == '-') {
					char aux_data = mDataString.at((unsigned _int64)k + 1);
					if (aux_data == '2') {
						mDataVector.push_back(-2);
						k++;
					}
					else if (aux_data == '1') {
						mDataVector.push_back(-1);
						k++;
					}
				}

			}
		}
		return mDataVector;
	}

	t_message_data_length getMessageDataLength(const t_message & msg) {

		if ((msg.messageDataLength).size() >= 0)
			return stoi(msg.messageDataLength);
		else
			return 0;
	}
}