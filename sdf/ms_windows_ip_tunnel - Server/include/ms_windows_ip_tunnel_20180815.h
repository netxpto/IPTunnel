# ifndef MS_WINDOWS_IP_TUNNEL_H_
# define MS_WINDOWS_IP_TUNNEL_H_

# include "../include/netxpto_20180815.h"

class IPTunnel : public Block {

public:

	// ##########################################################################

	IPTunnel(initializer_list<Signal*> InputSig, initializer_list<Signal*> OutputSig) : Block(InputSig, OutputSig) {};

	void initialize(void);
	bool runBlock(void);
	void terminate(void);

	void ipTunnelSendInt(int space);
	int ipTunnelRecvInt();

	template <class T>
	int ipTunnelPut(T object);

	// ##########################################################################

	void setDisplayNumberOfSamples(bool opt) { displayNumberOfSamples = opt; };
	bool getDisplayNumberOfSamples() const { return displayNumberOfSamples; };

	bool server();
	bool client();

	// ###########################################################################
	
private:

	// # Input Parameters ########################################################
	bool displayNumberOfSamples{ true};
	string remoteMachineIpAddress{ "127.0.0.1" };
	int tcpPort{ 54000 };
	int numberOfTrials{ 10 };
	int signalType{ 0 };
	int timeIntervalSeconds{ 3 };

	// # State Variables #########################################################

};

#endif
