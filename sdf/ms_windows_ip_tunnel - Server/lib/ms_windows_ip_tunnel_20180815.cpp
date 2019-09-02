# include "../include/ip_tunnel_ms_windows_20180815.h" 
#include "../include/message_processor_common_20190410.h"

constexpr int msgSegmentSize = 20000;
constexpr int sizeOfValArrays = 5000;

t_binary valArrayBin[sizeOfValArrays];
t_real valArrayReal[sizeOfValArrays];
t_complex valArrayComplex[sizeOfValArrays];
t_complex_xy valArrayComplexXY[sizeOfValArrays];
t_photon_mp_xy valArrayPhotonMPXY[sizeOfValArrays];

void IPTunnel::initialize(void)
{

	if (inputSignals.empty()) {
		printf("server%d\n", n++);
		if (!server()) {
			printf("Error opening server\n");
			//			::exit(1);
		}
	}
	else {
		printf("client%d\n", n++);
		if (!client()) {
			printf("Error opening client\n");
			//			::exit(1);
		}
	}
}


bool IPTunnel::runBlock(void)
{
	int ready, process;

	if (inputSignals.empty()) { //server

		signal_value_type sType = outputSignals[0]->getValueType();
		ipTunnelOutputData(outputSignals, sType);

		long space = outputSignals[0]->space();
		for (auto k : outputSignals) {
			long int aux = k->space();


			space = min(space, aux);
		}


		//----------------------------------------RECEIVING THE SIGNAL----------------------------------------

		process = ipTunnelRecvInt();

		if (process == 0) {
			//alive = false;
			if (displayNumberOfSamples) {
				cout << "Samples received through IP Tunnel: " << process << "\n";
			}
			return false;
		}

		std::clock_t start;
		double duration;

		start = std::clock();


		ipTunnelRecvValues(outputSignals, process, sType);
		
		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

		std::cout << "printf: " << duration << '\n';



		if (displayNumberOfSamples) {
			cout << "Samples received through IP Tunnel: " << process << "\n";
		}

		ipTunnelOutputData(outputSignals, sType);
	}
	else { //client
		signal_value_type sType = inputSignals[0]->getValueType();



		ready = inputSignals[0]->ready();



		process = (long int)ready;

		ipTunnelSendInt(process);

		if (process == 0) {
			//alive = false;
			if (displayNumberOfSamples) {
				cout << "Samples sent through IP Tunnel: " << process << "\n";
			}
			return false;
		}

		switch (sType) {
		case signal_value_type::t_binary: //SEND FILE
			std::clock_t start;
			double duration;

			start = std::clock();

			for (int k = 0; k < process; k++) {
				t_binary signalValue;
				inputSignals[0]->bufferGet(&signalValue);
				valArrayBin[k] = signalValue;
			}
			ipTunnelPut(valArrayBin, sizeof(t_binary), process);

			duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

			std::cout << "printf: " << duration << '\n';

			break;
		
		default:
			printf("Error sending signal due to signal type unknown\n");
			//::exit(1);
		}
		if (displayNumberOfSamples) {
			cout << "Samples sent through IP Tunnel: " << process << "\n";
		}
	}
	return true;
}

void IPTunnel::terminate(void) {
	closesocket(clientSocket);
	WSACleanup();
}

template <class T>
int IPTunnel::ipTunnelPut(T object) {
	char* tosend = (char*)& object;
	int remaining = sizeof(object);
	int result = 0;
	int sent = 0;
	while (remaining > 0) {
		result = send(clientSocket, tosend + sent, remaining, 0);
		if (result > 0) {
			remaining -= result;
			sent += remaining;
		}
		else if (result < 0) {
			printf("ERROR sending object!\n");
			// probably a good idea to close socket
			break;
		}
	}
	return 0;
}

template <class T, size_t N>
int IPTunnel::ipTunnelPut(T(&object)[N], int elementSize, int processf) {
	char* tosend = (char*)& object;
	int remaining = elementSize * processf;
	int result = 0;
	int sent = 0;
	while (remaining > 0) {
		result = send(clientSocket, tosend + sent, remaining, 0);
		if (result > 0) {
			remaining -= result;
			sent += remaining;
		}
		else if (result < 0) {
			printf("ERROR sending object!\n");
			// probably a good idea to close socket
			break;
		}
	}
	return 0;
}

void IPTunnel::ipTunnelSendInt(int space) {
	int data = space;
	char* tosend = (char*)& data;
	int remaining = sizeof(data);
	int result = 0;
	int sent = 0;
	while (remaining > 0) {
		result = send(clientSocket, tosend + sent, remaining, 0);
		if (result > 0) {
			remaining -= result;
			sent += remaining;
		}
		else if (result < 0) {
			//printf("ERROR sending int!\n");
			// probably a good idea to close socket
			break;
		}
	}
}

int IPTunnel::ipTunnelRecvInt() {
	int value = 0;
	char* recv_buffer = (char*)& value;
	int remaining = sizeof(int);
	int received = 0;
	int result = 0;
	while (remaining > 0) {
		result = recv(clientSocket, recv_buffer + received, remaining, 0);
		if (result > 0) {
			remaining -= result;
			received += result;
		}
		else if (result == 0) {
			printf("Remote side closed his end of the connection\n");
			// probably a good idea to close socket
			break;
		}
		else if (result < 0) {
			//printf("ERROR receiving int!\n");
			// probably a good idea to close socket
			break;
		}
	}
	return value;
}

bool IPTunnel::server() {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return false;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return false;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = ntohs((u_short)tcpPort);
	//inet_pton(AF_INET, (PCSTR)remoteMachineIpAddress.c_str(), &hint.sin_addr.s_addr); // hint.sin_addr.S_un.S_addr = inet_addr(ipAddressServer.c_str());
	hint.sin_addr.S_un.S_addr = INADDR_ANY;


	if (::bind(listening, (sockaddr*)& hint, sizeof(hint)) < 0) {
		printf("\n ERROR on binding");
		return false;
	}

	if (listen(listening, SOMAXCONN) == -1) {
		printf("\n ERROR on binding");
		return false;
	}

	bool setTcpNoDelay = true;
	setsockopt(listening, IPPROTO_TCP, TCP_NODELAY, (char*)& setTcpNoDelay, sizeof(setTcpNoDelay));

	sockaddr_in client;
	int clientSize = sizeof(client);

	clientSocket = accept(listening, (sockaddr*)& client, &clientSize);
	
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)& client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}
	return true;
}

bool IPTunnel::client() {

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return false;
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons((u_short)tcpPort);
	inet_pton(AF_INET, remoteMachineIpAddress.c_str(), &hint.sin_addr);

	bool setTcpNoDelay = true;
	setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, (char*)& setTcpNoDelay, sizeof(setTcpNoDelay));

	int connResult = -2;
	while (connResult != 0 || numberOfTrials == 0) {
		connResult = connect(clientSocket, (sockaddr*)& hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			cerr << "Waiting " << timeIntervalSeconds << " seconds." << endl;
			Sleep(timeIntervalSeconds * 1000);
		}

		if (--numberOfTrials == 0) {
			cerr << "Reached maximum number of attempts." << endl;
			//::exit(1);
		}
	}
	cout << "Connected!\n";
	return true;
}


bool IPTunnel::ipTunnelRecvValues(vector <Signal*> outputS, int processf, signal_value_type stypef) {
	int result = 0;
	int remaining = 0;
	char* recv_buffer = 0;

	switch (stypef) {
	case signal_value_type::t_binary: //RECEIVE FILE

		
		recv_buffer = (char*)& valArrayBin;
		remaining = sizeof(t_binary) * processf;
		break;
	
	default:
		printf("Error getting signal type\n");
		//::exit(1);
	}

	int received = 0;
	result = 0;
	while (remaining > 0) {
		result = recv(clientSocket, recv_buffer + received, remaining, 0);
		if (result > 0) {
			remaining -= result;
			received += result;
		}
		else if (result == 0) {
			printf("Remote side closed his end of the connection before all data was received\n");
			// probably a good idea to close socket
			break;
		}
		else if (result < 0) {
			printf("ERROR!\n");
			// probably a good idea to close socket
			break;
		}
	}

	switch (stypef) {
	case signal_value_type::t_binary:
		vBinary.insert(vBinary.end(), valArrayBin, valArrayBin + processf);
		break;
	default:
		printf("Error putting signal in buffer due to signal type unknown\n");
		//::exit(1);
	}

	return true;

}

bool IPTunnel::ipTunnelOutputData(vector <Signal*> outputS, signal_value_type sType) {
	bool alive = false;

	int space = outputS[0]->getBufferLength();

	for (auto k : outputS) space = (space < k->space()) ? space : k->space();
	int processf;
	switch (sType) {
	case signal_value_type::t_binary:
		processf = min(space, (int)vBinary.size());
		for (int k = 0; k < processf; ++k) {
			for (unsigned int s = 0; s < outputS.size(); ++s) {
				outputS[s]->bufferPut(vBinary[k]);
			}
		}
		vBinary.erase(vBinary.begin(), vBinary.begin() + processf);
		break;
	default:
		auto outputVec = { 0 };
		printf("Error putting signal in buffer due to signal type unknown\n");
		//::exit(1);
	}

	alive = true;
	return alive;
}