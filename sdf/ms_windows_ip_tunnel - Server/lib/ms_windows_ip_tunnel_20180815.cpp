# include "../include/ms_windows_ip_tunnel_20180815.h" 

#include <WS2tcpip.h> //official windows header with some functions neededd, 
#pragma comment(lib, "ws2_32.lib") //link the winsock library file, can also link Settings->linker->Additional dependency

SOCKET clientSocket;

void IPTunnel::initialize(void)  //crie aqui o servidor e o cliente
{
	if (inputSignals.empty()) {
		printf("server");
		if (!server()) {
			printf("Error opening server\n");
			exit(1);
		}
		signalType = ipTunnelRecvInt();
	}
	else {
		signal_value_type sType = inputSignals[0]->getValueType();
		printf("client");
		if (!client()) {
			printf("Error opening client\n");
			exit(1);
		}
		switch (sType) {
			case signal_value_type::t_binary: //1
				signalType = 1;
				break;
			case signal_value_type::t_real: //2
				signalType = 2;
				break;
			case signal_value_type::t_complex: //3
				signalType = 3;
				break;
			case signal_value_type::t_complex_xy: //4
				signalType = 4;
				break;

			case signal_value_type::t_photon_mp_xy: //5
				signalType = 5;
				break;
			default:
				printf("Error getting signal type\n");
				exit(1);
		}
		ipTunnelSendInt(signalType);
		
	}
}


bool IPTunnel::runBlock(void)
{
	int ready;
	int process;
	
	if (inputSignals.empty()) { //server
		
		long space = outputSignals[0]->space();
		for (auto k : outputSignals) {
			long int aux = k->space();
			space = min(space, aux);
		}
		
		
		ipTunnelSendInt(space);


		//printf("waiting to receive the signal...\n");
		//----------------------------------------RECEIVING THE SIGNAL----------------------------------------


		process = ipTunnelRecvInt();

		if (process == 0) {
			//alive = false;
			if (displayNumberOfSamples) {
				cout << "Samples received through IP Tunnel: " << process << "\n";
			}
			return false;
		}

		t_binary valueBinary;
		t_real valueReal;
		t_complex valueComplex;
		t_complex_xy valueComplexXy;
		t_photon_mp_xy valueComplexMp;


		int result = 0;
		int remaining;
		char* recv_buffer = 0;

		for (int k = 0; k < process; k++) {
			switch (signalType) {
				case 1: //signal_value_type::t_binary:
					recv_buffer = (char*)&valueBinary;
					remaining = sizeof(t_binary);
					break;
				case 2: //signal_value_type::t_real: 
					recv_buffer = (char*)&valueReal;
					remaining = sizeof(t_real);
					break;
				case 3: //signal_value_type::t_complex: 
					recv_buffer = (char*)&valueComplex;
					remaining = sizeof(t_complex);
					break;
				case 4: //signal_value_type::t_complex_xy: 
					recv_buffer = (char*)&valueComplexXy;
					remaining = sizeof(t_complex_xy);
					break;

				case 5: //signal_value_type::t_photon_mp_xy:
					recv_buffer = (char*)&valueComplexMp;
					remaining = sizeof(t_photon_mp_xy);
					break;
				default:
					printf("Error getting signal type\n");
					exit(1);
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
			switch (signalType) {
				case 1:
					outputSignals[0]->bufferPut(valueBinary);
					break;
				case 2: 
					outputSignals[0]->bufferPut(valueReal);
					break;
				case 3: 
					outputSignals[0]->bufferPut(valueComplex);
					break;
				case 4:
					outputSignals[0]->bufferPut(valueComplexXy);
					break;
				case 5:
					outputSignals[0]->bufferPut(valueComplexMp);
					break;
				default:
					printf("Error putting signal in buffer due to signal type unknown\n");
					exit(1);
			}
		}
		//printf("Signal Received and sent to buffer\n");

		if (displayNumberOfSamples) {
			cout << "Samples received through IP Tunnel: " << process << "\n";;
		}
	}
	else { //client
		ready = inputSignals[0]->ready();
		int space = ipTunnelRecvInt();
		
		process = min((long int)ready, space);
		ipTunnelSendInt(process);

		if (process == 0) {
			//alive = false;
			if (displayNumberOfSamples) {
				cout << "Samples sent through IP Tunnel: " << process << "\n";
			}
			return false;
		}

		//printf("space of received IPTunnel:%d\n", space);
		//printf("process:%d\n", process);

		switch (signalType) {
			case 1:
				for (int k = 0; k < process; k++) {
					t_binary signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
			case 2:
				for (int k = 0; k < process; k++) {
					t_real signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
			case 3:
				for (int k = 0; k < process; k++) {
					t_complex signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
			case 4:
				for (int k = 0; k < process; k++) {
					t_complex_xy signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;

			case 5:
				for (int k = 0; k < process; k++) {
					t_photon_mp_xy signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
			default:
				printf("Error sending signal due to signal type unknown\n");
				exit(1);
		}

		if (displayNumberOfSamples) {
			cout << "Samples sent through IP Tunnel: " << process << "\n";
		}

	}


	//Client
	/*
	if (process == 0 && !alive) { //&& ready == 0
		//SEND ALIVE TO SERVER
		return false;
	}
	else if (process == 0) {
		alive = false;
		return false;
	}*/

	return true; 
	
}

void IPTunnel::terminate(void) {
	closesocket(clientSocket);
	WSACleanup();
}


template <class T>
int IPTunnel::ipTunnelPut(T object){
	char* tosend = (char*)&object;
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
			printf("ERROR!\n");
			// probably a good idea to close socket
			break;
		}
	}

	return 0;

}


void IPTunnel::ipTunnelSendInt(int space) {
	int data = space;
	char* tosend = (char*)&data;
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
			printf("ERROR!\n");
			// probably a good idea to close socket
			break;
		}
	}
}

int IPTunnel::ipTunnelRecvInt() {
	int value = 0;
	char* recv_buffer = (char*)&value;
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
	return value;
}

bool IPTunnel::server() {
	//SERVER -------------------------------------------------------------------------
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsOk = WSAStartup(ver, &wsData);
	if (wsOk != 0)
	{
		cerr << "Can't Initialize winsock! Quitting" << endl;
		return false;
	}

	// Create a socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return false;
	}

	// Bind the ip address and port to a socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = ntohs(tcpPort);
	//hint.sin_addr.S_un.S_addr = inet_addr(ipAddressServer.c_str());
	inet_pton(AF_INET, (PCSTR)"127.0.0.1", &hint.sin_addr.s_addr);// inet_addr("127.0.0.1");


	if (::bind(listening, (sockaddr*)&hint, sizeof(hint)) < 0) {
		printf("\n ERROR on binding");
		return false;
	}
	// Tell Winsock the socket is for listening 


	if (listen(listening, SOMAXCONN) == -1) {
		printf("\n ERROR on binding");
		return false;
	}
	// Wait for a connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST];		// Client's remote name
	char service[NI_MAXSERV];	// Service (i.e. port) the client is connect on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);
	
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	// Close listening socket
	//closesocket(listening);

	/*
	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");*/
	//SERVER -------------------------------------------------------------------------
}

bool IPTunnel::client() {
	//CLIENTE -------------------------------------------------------------------------

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		cerr << "Can't start Winsock, Err #" << wsResult << endl;
		return false;
	}

	// Create socket
	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return false;
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(tcpPort);
	inet_pton(AF_INET, remoteMachineIpAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = -2;
	while (connResult != 0 ) {
		connResult = connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
		printf("%d\n", connResult);
		if (connResult == SOCKET_ERROR)
		{
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			//closesocket(sock);
			//WSACleanup();
			//return false;
		}

		Sleep(3*1000);
	}
	cout << "Connected!\n";
	/*
	// Do-while loop to send and receive data
	char buf[4096];
	string userInput;

	do
	{
		// Prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0)		// Make sure the user has typed in something
		{
			// Send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					// Echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
			}
			else {
				cerr << "Got no reply, Err #" << WSAGetLastError() << endl;
			}
		}

	} while (userInput.size() > 0);

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();*/
	//CLIENTE ---------------------------------------------------------------
}