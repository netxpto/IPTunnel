# include "../include/ip_tunnel_20180815.h"
#pragma warning(disable:4996) 
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

SOCKET createConnection(char*);
SOCKET createListeningSocket();

bool server();
bool client();

bool IPTunnel::runBlock(void)
{

	int ready = inputSignals[0]->ready(); //int ready2 = inputTCPConnetion[0]->ready();

	cout << "---------------- IP Tunnel ----------------------\n";
	int process;
	if (numberOfSamples >= 0) {
		process = min((long int)ready, numberOfSamples);
	}
	else {
		process = ready;
	}

	if (process == 0) {
		alive = false;
		return alive; //blocked = true;
	}
	else {
		outputSignals[0]->bufferPut((t_binary)ready); //process;
		for (int i = 0; i < process; i++) {
			inputSignals[0]->bufferGet();
			++processedSamples;
		}

	}


	if (numberOfSamples >= 0) numberOfSamples -= process;

	if (displayNumberOfSamples) {
		cout << "ip tunnel Samples to receive: " << 0 << "\n";
		cout << "ip tunnel Samples to send: " << process << "\n";
	}


	return false; //~(blocked); // & entangledBlocked());
	

	/*
	
	WSADATA wsaData;

	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	//server
	SOCKET sockfd, newsockfd;
	int portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	char sendbuf[15] = "this is a test";
	char recvbuf[256];
	int resul;
	int num;


	// create a socket(int domain, int type, int protocol)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		fprintf(stderr, "Socket failure!!\n");
		//exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, sendbuf, sizeof(int)) == -1) {
		perror("setsockopt");
		//exit(1);
	}

	// clear address structure
	bzero((char *)&serv_addr, sizeof(serv_addr));

	portno = 8080;

	//setup the host_addr structure for use in bind call 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.1.8");//
	serv_addr.sin_port = htons(portno);

	if (::bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("\n ERROR on binding");
		//exit(1);
	}

	if (listen(sockfd, 10) == -1) {
		fprintf(stderr, "Listening Failure\n");
		//exit(1);
	}


	while (1) {


		if ((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen)) == -1) {
			perror("accept");
			//exit(1);
		}
		printf("Server got connection from client %s\n", inet_ntoa(serv_addr.sin_addr));

		while (1) {

			if ((num = recv(newsockfd, buffer, 1024, 0)) == -1) {
				perror("recv");
				//exit(1);
			}
			else if (num == 0) {
				printf("Connection closed\n");
				//So I can now wait for another client
				break;
			}
			buffer[num] = '\0';
			printf("Server:Msg Received %s\n", buffer);
			if ((send(newsockfd, buffer, strlen(buffer), 0)) == -1)
			{
				fprintf(stderr, "Failure Sending Message\n");
				//close(newsockfd);
				break;
			}

			printf("Server:Msg being sent: %s\nNumber of bytes sent: %d\n", buffer, strlen(buffer));

		} //End of Inner While...
		//Close Connection Socket
		//close(newsockfd);
	} //Outer While

	//close(sockfd);
	return 0;
	*/


	/*
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		printf("ERROR on accept");

	printf("server: got connection from %s port %d\n",inet_ntop(serv_addr.sin_family,
		&serv_addr.sin_addr.s_addr, buffer, clilen), portno);
	printf("Connecting to '%d.%d.%d.%d', port %d\n",
		serv_addr.sin_addr.s_addr & 0xFF,
		(serv_addr.sin_addr.s_addr >> 8) & 0xFF,
		(serv_addr.sin_addr.s_addr >> 16) & 0xFF,
		(serv_addr.sin_addr.s_addr >> 24) & 0xFF,
		portno);
		*/
	/*//client
	SOCKET socketId = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddr;
	socklen_t addrSize = sizeof(serverAddr);
	bzero((char*)&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5505);
	serverAddr.sin_addr.s_addr = inet_addr("192.168.1.8");//INADDR_ANY;
	resul = connect(newsockfd, (struct sockaddr*)&serverAddr, addrSize);
	//resul = setsockopt(newsockfd, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);

	printf("Trying to connect to '%d.%d.%d.%d', port %d\n",
		serverAddr.sin_addr.s_addr & 0xFF,
		(serverAddr.sin_addr.s_addr >> 8) & 0xFF,
		(serverAddr.sin_addr.s_addr >> 16) & 0xFF,
		(serverAddr.sin_addr.s_addr >> 24) & 0xFF,
		portno);
	if (resul == SOCKET_ERROR) {
		printf("connect failed: %d\n", WSAGetLastError());
		closesocket(newsockfd);
		WSACleanup();
		return 1;
	}
	resul = send(newsockfd, sendbuf, (int)(sendbuf), 0);
	if (resul == SOCKET_ERROR) {
		printf("send failed: %d\n", WSAGetLastError());
		closesocket(newsockfd);
		WSACleanup();
		return 1;
	}
	printf("Bytes Sent: %ld\n", resul);

	do{
		resul = recv(newsockfd, recvbuf, 256, 0);
		if (resul > 0)
			printf("Bytes received: %d\n", resul);
		else if (resul == 0)
			printf("Connection closed\n");
		else
			printf("recv failed: %d\n", WSAGetLastError());
	} while (resul > 0);


	bzero(buffer, 256);
	

	resul = recv(socketId, buffer, 256, 0);

	fprintf(stdout, "%s %s\n", "Response from server", resul);

	while (true) { Sleep(1000); }

	

	//close(socketId);



	//n = _read(newsockfd, buffer, 255);
	//if (n < 0) printf("ERROR reading from socket");
	//printf("Here is the message: %s\n", buffer);
	
	//close(newsockfd);
	//close(sockfd);
	return 0;*/
}

bool server() {
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
		//exit(1);
	}
	// Tell Winsock the socket is for listening 


	if (listen(listening, SOMAXCONN) == -1) {
		printf("\n ERROR on binding");
		//exit(1);
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
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return false;
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
		}

	} while (userInput.size() > 0);

	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
	//CLIENTE ---------------------------------------------------------------
}

SOCKET createConnection(char *ipaddr)
{
	SOCKET sockfd = 0;
	struct sockaddr_in serv_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket on \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(65432);

	if (inet_pton(AF_INET, ipaddr, &serv_addr.sin_addr) <= 0)
	{
		printf("\n inet_pton error occured\n");
		return -1;
	}

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return -1;
	}

	return sockfd;
}

SOCKET createListeningSocket()
{
	SOCKET listenfd = 0;
	struct sockaddr_in serv_addr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	cout << "ip tunnel Samples to send: " << listenfd << "\n";
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

	bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(listenfd, 10);

	return listenfd;
}

