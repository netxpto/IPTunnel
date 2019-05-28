# include "../include/ip_tunnel_20180815.h" 


#pragma warning(disable:4996) //inet_addr()
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <string> 

SOCKET clientSocket;

void IPTunnel::initialize(void)  //crie aqui o servidor e o cliente
{
	if (inputSignals.empty()) {
		if (!server()) {
			printf("Error opening server\n");
			exit(1);
		}
	}
	else {

		if (!client()) {
			printf("Error opening client\n");
			exit(1);
		}
	}
}


bool IPTunnel::runBlock(void)
{
	int ready;
	int process;
	
	//client ------------> server

	//no inicio do run deve atualizar as variavies ready e space (ipTunnelSpace()) process = min(ready, space)
	if (inputSignals.empty()) { //server
		
		long space = outputSignals[0]->space();
		for (auto k : outputSignals) {
			long int aux = k->space();
			space = min(space, aux);
		}
		printf("sending space to client\n");
		ipTunnelSendInt(space);

		printf("waiting to receive the signal...\n");

		printf("Signal Received!!!\n");

	}
	else { //client
		ready = inputSignals[0]->ready();
		int space = ipTunnelRecvInt();
		
		process = min((long int)ready, space);
		ipTunnelSendInt(process);
		printf("space of received IPTunnel:%d\n", space);
		printf("process:%d\n", process);

		signal_value_type sType = inputSignals[0]->getValueType();
		switch (sType) {
			case signal_value_type::t_binary: //1
				ipTunnelSendInt(1);
				printf("binary");
				for (int k = 0; k < process; k++) {
					
					t_binary signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
			case signal_value_type::t_real: //2
				ipTunnelSendInt(2);
				printf("t_real");
				for (int k = 0; k < process; k++) {
					t_real signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
			case signal_value_type::t_complex: //3
				ipTunnelSendInt(3);
				printf("t_complex");
				for (int k = 0; k < process; k++) {
					t_complex signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
			case signal_value_type::t_complex_xy: //4
				ipTunnelSendInt(4);
				printf("t_complex_xy");
				for (int k = 0; k < process; k++) {
					t_complex_xy signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;

			case signal_value_type::t_photon_mp_xy: //5
				ipTunnelSendInt(5);
				printf("t_complex_mp_xy");
				for (int k = 0; k < process; k++) {
					t_photon_mp_xy signalValue;
					inputSignals[0]->bufferGet(&signalValue);
					ipTunnelPut(signalValue);
				}
				break;
		}

		while (true) { Sleep(1000); }
	}


	

	
	//std::cout << std::bitset<64>(ready);
	
	//ready = ready << 2;
	
	//int temp = ready;

	cout << "---------------- IP Tunnel ----------------------\n";
	cout << "---------------- IP Tunnel ----------------------\n";
	printf("%d\n",ready);
	printf("%d\n", numberOfSamples);
	printf("%d\n", inputSignals[0]);
	auto temp = inputSignals[0];
	//int process;
	if (numberOfSamples >= 0) {
		process = min((long int)ready, numberOfSamples);
	}
	else {
		process = ready;
	}
	printf("process:%d\n", process);
	if (process == 0) {
		alive = false;
		//return alive; //blocked = true;
	}
	else {
		outputSignals[0]->bufferPut((t_binary)ready); //process;
		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet();
			++processedSamples;
		}

	}
	printf("process:%d\n", process);

	if (numberOfSamples >= 0) numberOfSamples -= process;

	if (displayNumberOfSamples) {
		cout << "ip tunnel Samples to receive: " << 0 << "\n";
		cout << "ip tunnel Samples to send: " << process << "\n";
	}
	printf("%d\n", inputSignals[0]);
	
	if (process == 0) {
		alive = false;
		//update alive in the other block

	}
	else {

	}

	return false; //~(blocked); // & entangledBlocked());
	
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
		printf("Remaining to send:%d\n", remaining);
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
	// Initialze winsock
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
	hint.sin_port = htons(tcpPort);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; // inet_addr("127.0.0.1");//Could also use inet_pton .... 


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
	closesocket(listening);

	/*
	// While loop: accept and echo message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);


		int sen = send(clientSocket, (char*)&inputSignal, sizeof(inputSignal), 0);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << WSAGetLastError() << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		cout << string(buf, 0, bytesReceived) << endl;

		// Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);

	}

	// Close the socket
	closesocket(clientSocket);

	// Cleanup winsock
	WSACleanup();

	system("pause");*/
	//SERVER -------------------------------------------------------------------------
}

bool IPTunnel::client() {
	//CLIENTE -------------------------------------------------------------------------
	//string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = tcpPort;						// Listening port # on the server

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
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

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

		Sleep(3000);
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