# ifndef IP_TUNNEL_H_
# define IP_TUNNEL_H_

# include "../include/netxpto_20180815.h"

class IPTunnel : public Block {

public:

	// ##########################################################################

	IPTunnel(initializer_list<Signal*> InputSig, initializer_list<Signal*> OutputSig) : Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);

	void ipTunnelSendInt(int space);
	int ipTunnelRecvInt();

	template <class T>
	int ipTunnelPut(T object);

	// ##########################################################################

	void setNumberOfSamples(long int nOfSamples) { numberOfSamples = nOfSamples; };
	long int getNumberOfSamples() const { return numberOfSamples; };

	void setProcessedSamples(long int pSamples) { processedSamples = pSamples; };
	long int getProcessedSamples() const { return processedSamples; };

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };
	bool getDisplayNumberOfSamples() const { return displayNumberOfSamples; };

	bool server();
	bool client();

	// ###########################################################################
	
private:

	// # Input Parameters ########################################################
	long int numberOfSamples{ -1 };
	long int processedSamples{ 0 };
	bool displayNumberOfSamples{ true};
	//const char* ipAddressServer{ "127.0.0.1" };
	string ipAddressServer{ "127.0.0.1" };
	int tcpPort{ 54000 };
	int numberOfTrials{ 5 };
	int numberOfRepetions{ 3 };

	// # State Variables #########################################################
	bool alive{ true };
	bool finished{ false };

};

#endif
