# ifndef IP_TUNNEL_H_
# define IP_TUNNEL_H_

# include "../include/netxpto_20180815.h"

class IPTunnel : public Block {

public:

	// ##########################################################################

	IPTunnel(initializer_list<Signal*> InputSig, initializer_list<Signal*> OutputSig) : Block(InputSig, OutputSig) {};
	bool runBlock(void);

	// ##########################################################################

	void setNumberOfSamples(long int nOfSamples) { numberOfSamples = nOfSamples; };
	long int getNumberOfSamples() const { return numberOfSamples; };

	void setProcessedSamples(long int pSamples) { processedSamples = pSamples; };
	long int getProcessedSamples() const { return processedSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };
	bool getDisplayNumberOfSamples() const { return displayNumberOfSamples; };

	// ###########################################################################
	
private:

	// # Input Parameters ########################################################
	t_ip_address ipAddress{ -1 };
	t_tcp_port tcpPort{};
	int numberOfTrials{ 5 };
	int numberOfRepetions{ 3 };

	// # State Variables #########################################################
	bool blocked{ false };

};

#endif
