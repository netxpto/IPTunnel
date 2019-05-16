# include "../include/ip_tunnel_20180815.h"

bool IPTunnel::runBlock(void)
{
	int ready = inputSignals[0]->ready(); //int ready2 = inputTCPConnetion[0]->ready();

	/*// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address,
		sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
		(socklen_t*)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	return 0;

	*/

	cout << "---------------- IP Tunnel ----------------------\n";
	int process;
	if (numberOfSamples >= 0) {
		process = std::min((long int)ready, numberOfSamples);
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

}
