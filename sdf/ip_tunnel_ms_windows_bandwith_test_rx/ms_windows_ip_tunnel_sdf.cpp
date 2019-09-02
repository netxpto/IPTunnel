# include "include/netxpto_20180815.h"
# include "include/ip_tunnel_ms_windows_20180815.h"
# include "include/binary_source_20180815.h"
# include "include/sink_20180815.h"


// #####################################################################################################
// ################################### System Input Parameters #########################################
// #####################################################################################################

int numberOfBits{ 1601 };	

int main() {

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	
	Binary IPTunnelSourceOut_3{ "S2_BinarySourceOut_0.sgn" };

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	
	IPTunnel ip_tunnel{ {},{&IPTunnelSourceOut_3} };
	//ip_tunnel.setTcpPort(54000);
	//ip_tunnel.setRemoteMachineIpAddress("127.0.0.1");

	ip_tunnel.setDisplayNumberOfSamples(false);

	Sink Sink_0{ {&IPTunnelSourceOut_3},{} };

	Sink_0.setDisplayNumberOfSamples(false);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ 
		{ 
			&ip_tunnel,
			&Sink_0
		}
	};

	MainSystem.setLogValue(true);
	MainSystem.setSaveSignals(true);

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	std::clock_t start;
	double duration;

	start = std::clock();

	MainSystem.run();


	MainSystem.terminate();

	duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

	std::cout << "duration of the simulation: " << duration << '\n';

	system("pause");
	return 0;

}