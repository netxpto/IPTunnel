# include "../include/ip_tunnel_20180815.h"

bool IpTunnel::runBlock(void)
{
	int ready = inputTCPConnetion[0]->ready();

	int process;
	if (numberOfSamples >= 0) {
		process = std::min((long int)ready, numberOfSamples);
	}
	else {
		process = ready;
	}
	 
	if (process == 0) blocked = true;

	return ~(blocked & entangledBlocked()) ;
}
