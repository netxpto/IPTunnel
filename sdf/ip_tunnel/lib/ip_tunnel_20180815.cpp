# include "../include/ip_tunnel_20180815.h"

#pragma warning(disable:4996) 
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#include <string> 

bool server(Signal*);
bool client();

void IPTunnel::initialize(void)  //crie aqui o servidor e o cliente
{

}


bool IPTunnel::runBlock(void)
{
	int ready;

	
	if (inputSignals.empty()) {
		if (!server(inputSignals[0])) {
			printf("Error opening server\n");
			exit(1);
		}
	}
	else {
		ready = inputSignals[0]->ready(); //int ready2 = inputTCPConnetion[0]->ready();

		if (!client()) {
			printf("Error opening client\n");
			exit(1);
		}
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
	int process;
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

bool server(Signal* inputSignal) {
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
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");//INADDR_ANY; // Could also use inet_pton .... 


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

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

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

	system("pause");
	//SERVER -------------------------------------------------------------------------
}

bool client() {
	//CLIENTE -------------------------------------------------------------------------
	string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 54000;						// Listening port # on the server

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
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
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
		connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
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
	WSACleanup();
	//CLIENTE ---------------------------------------------------------------
}


