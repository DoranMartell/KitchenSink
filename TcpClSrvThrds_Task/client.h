#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <iostream>
#include <iomanip>
#include <chrono>

#define BUFF_SIZE 1024

using std::cerr;
using std::endl;

class Client
{
public:
	
	int StartClient(const char * name_of_client, const char * port_num, const char * period_to_wait);
	void SendMessageLoop();

private:

	int FillClientNameAndPeriod(const char * name_from_args, const char * time_from_args);
	int CreateSocket();
	int BuildDestinationAddr(int port);
	int Connect();

	const char * client_name;
	int period;
	int dest_socket;
	struct sockaddr_in destination_addr;

	void BuildMessage(char * message, int capacity, int & actual_size);

};

void brokn_pipe_handler(int signum);