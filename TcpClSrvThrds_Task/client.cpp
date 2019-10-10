#include "client.h"

int Client::FillClientNameAndPeriod(const char * name_from_args, const char * time_from_args)
{
	client_name = name_from_args;
	period = atoi(time_from_args);

	if (period < 0)
		return -1;

	return 0;
}

int Client::CreateSocket()
{
	dest_socket = socket(AF_INET, SOCK_STREAM, 0);
	return dest_socket; 
}

int Client::BuildDestinationAddr(int port)
{
	if (period < 0 || port > 65535)
		return -1;

	memset((char *) & destination_addr, '\0', sizeof(destination_addr));
	destination_addr.sin_family = AF_INET;
	destination_addr.sin_port = htons(port);
	inet_aton("127.0.0.1", &destination_addr.sin_addr);

	return 0;
}

int Client::Connect()
{
	return connect(dest_socket, (struct sockaddr *) & destination_addr, sizeof(destination_addr));
}

void Client::BuildMessage(char * message, int capacity, int & actual_size)
{	
	memset(message, '\0', capacity);

	time_t rawtime;
    tm * timeinfo;
    char time_buffer[80];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(time_buffer, 80,"%Y-%m-%d %H:%M:%S", timeinfo);

    actual_size = sprintf(message, "[%s] %s\n", time_buffer, client_name);

}

void Client::SendMessageLoop()
{
	char message[BUFF_SIZE];

	signal(SIGPIPE, brokn_pipe_handler);

	while (1)
	{
		int actual_size = 0;
		BuildMessage(message, sizeof(message), actual_size);

		if (write(dest_socket, message, actual_size) <= 0)
		{
			cerr << "Apparently, server is disconnected\n";
			return;
		}



		sleep(period);
	}
}

int Client::StartClient(const char * name_of_client, const char * port_num, const char * period_to_wait)
{
	if (FillClientNameAndPeriod(name_of_client, period_to_wait) < 0)
	{
		cerr << "Apparently, period " << period_to_wait << " is not correct" << endl;
		return -1;
	}

	if (CreateSocket() < 0)
	{
		cerr << "Unable to create socket: " << strerror(errno) << endl;
		return -2;
	}

	if (BuildDestinationAddr(atoi(port_num)) < 0)
	{
		cerr << "Apparently, port number " << port_num << " is not correct" << endl;
		return -3;
	}

	if (Connect() < 0)
	{
		cerr << "Error while connecting: " << strerror(errno) << endl;
		return -4;
	}
}

void brokn_pipe_handler(int signum)
{
	cerr << "Signal " << signum << " received. Apparently, server is disconnected\n";
	exit(signum);
}

int main(int argc, char const * argv[])
{
	if (argc != 4)
	{
		cerr << "Please pass 3 arguments: <name> <destination port> <period of message sending>" << endl;
		exit(1);
	}

	Client client;
	int why;
	if ((why = client.StartClient(argv[1], argv[2], argv[3])) < 0)
	{
		exit(why);
	}

	client.SendMessageLoop();
	
	return 0;
}