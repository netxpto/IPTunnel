#ifndef MESSAGE_PROCESSOR_COMMON_H_
#define MESSAGE_PROCESSOR_COMMON_H_

#include <vector>
#include "../../../include/netxpto_20180815.h"

typedef enum { BasisReconciliationMsg, ErrorCorrectionMsg, ErrorCorrectionPermutationsMsg, PrivacyAmplificationSeedsMsg, ParameterEstimationSeedAndBitsMsg, ToggleBERChangeMsg} t_message_type;
typedef int t_message_data_length;
typedef vector<int> t_message_data;

namespace MessageProcessors {
	t_message_data getMessageData(const t_message& msg, t_message_data_length dataLength);
	string generateMessageData(const t_message_data &msgData, const t_message_type &msgType);
	t_message_data_length getMessageDataLength(const t_message& msg);
	t_message_type getMessageType(const t_message& msg);
	bool sendToMsgProc(vector <t_integer>& data, Signal& signalToMsg, bool& started);
	bool recvFromMsgProc(vector <t_integer>& data, Signal& signalToMsg, bool& started, int& numberofValuesToRecv);
	
}


#endif // MESSAGE_PROCESSOR_COMMON_H_