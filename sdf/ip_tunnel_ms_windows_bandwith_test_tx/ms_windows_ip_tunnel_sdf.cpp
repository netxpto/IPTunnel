# include "include/netxpto_20180815.h"
# include "include/ip_tunnel_ms_windows_20180815.h"
# include "include/binary_source_20180815.h"
# include "include/sink_20180815.h"


// #####################################################################################################
// ################################### System Input Parameters #########################################
// #####################################################################################################

long numberOfBits{ 8000000 };

int main() {

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	Binary BinarySourceOut_0{ "S0_BinarySourceOut_0.sgn" };

	Binary BinarySourceOut_1{ "S1_BinarySourceOut_0.sgn" };

	Binary BinarySourceOut_2{ "S2_BinarySourceOut_0.sgn" };

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################


	BinarySource BinarySource_{ {}, { &BinarySourceOut_0, &BinarySourceOut_1, &BinarySourceOut_2 } };
	BinarySource_.setBitPeriod(1e-6);
	BinarySource_.setNumberOfBits(numberOfBits);
	
	IPTunnel IpTunnel_{ {&BinarySourceOut_1},{} };
	//IpTunnel_.setTcpPort(54000);
	//IpTunnel_.setRemoteMachineIpAddress("127.0.0.1");
	IpTunnel_.setDisplayNumberOfSamples(false);

	Sink Sink_1{ {&BinarySourceOut_0},{} };
	Sink_1.setDisplayNumberOfSamples(false);
	Sink Sink_2{ {&BinarySourceOut_2},{} };
	Sink_2.setDisplayNumberOfSamples(false);
	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ 
		{ 
			&BinarySource_,
			&IpTunnel_,
			&Sink_1,
			&Sink_2

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