#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring> /* for memset() and strcmp() functions */
#include <fcntl.h>

#define BUFSIZE 256

using namespace std;

int p; /* port number */
struct sockaddr_in serveraddr;

std::mutex mu;
bool finishCaught;

void PrintMessage(string msg)
{
	mu.lock();
	cout << "SERVER: " << msg << endl;
	mu.unlock();
}

void ChangeFinishCaught(bool val)
{
	mu.lock();
	finishCaught = val;
	mu.unlock();
}

void BuildServerAddr()
{
	/* building server's internet address */
	memset((char *) &serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons((unsigned short) p);
}

void AskPort()
{
	p = 0;
	while (p <= 0 || p > 65000)
	{
		cout << "SERVER: Please, enter a port number: ";
		cin >> p;
		cout << endl;
	}

	/* building server's internet address */
	BuildServerAddr();
}

void echo(int socket_descriptor, string incom_ip, int incom_port, int thread_num)
{
	int n;
	char buf[BUFSIZE];
	memset(buf, '\0', BUFSIZE);

	string mess_to_print;

	while (1)
	{
		if (finishCaught)
		{
			memset(buf, '\0', BUFSIZE);
			return;
		}
		else
		{
			/* read: get what was sent by client */
			n = read(socket_descriptor, buf, BUFSIZE);

			if (n < 0)
			{
				//mess_to_print = "ERROR reading from socket " + to_string(socket_descriptor);
				//PrintMessage(mess_to_print);
				usleep(5000);
			}
			else
			{	
				mess_to_print = "Socket " + to_string(socket_descriptor) + " received the following: " + string(buf)
								+ " from " + incom_ip + ":" + to_string(incom_port);
				PrintMessage(mess_to_print);

				if (!strcmp(buf, "exit") || !strcmp(buf, "quit") || !strcmp(buf, "finish") || !strcmp(buf, "finish\n"))
				{
					mess_to_print = string(buf) + " was caught! Server is going to shutdown!";
					PrintMessage(mess_to_print);
					ChangeFinishCaught(true);
					memset(buf, '\0', BUFSIZE);
					return;
				}

				/* write: echo the input string back to the client */
				n = write(socket_descriptor, buf, strlen(buf));
				if (n < 0) 
				{
					mess_to_print = "ERROR writing to socket " + to_string(socket_descriptor);
					PrintMessage(mess_to_print);
				}

				memset(buf, '\0', BUFSIZE);
			}
		}
	}
}

int main(int argc, char const *argv[])
{

	int listen_sock;

	struct sockaddr_in clientaddr; /* client addr */
	socklen_t clientlen;

	vector<int> conn_sock(2); /* maximum two clients */
	vector<string> clientsIP(2);
	vector<int> clientsPort(2);

	vector<std::thread> t(2); /* array of threads - one thread per each client */
	string mess_to_print;

	/* creating a socket for listening */
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock < 0) 
	{
		mess_to_print = "ERROR opening socket for listening";
		PrintMessage(mess_to_print);
	}

	AskPort();

	/* bind: associate the listening socket with a port */
	while (bind(listen_sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0) 
	{
		mess_to_print = "ERROR on binding to " + to_string(p) + " port";
		PrintMessage(mess_to_print);
		AskPort();
	}

	/* listen: make it a listening socket ready to accept connection requests */
	if (listen(listen_sock, 5) < 0) /* allow 5 requests to queue up */ 
	{
		mess_to_print = "ERROR on listen";
		PrintMessage(mess_to_print);
	}

	int number_of_active_sock = 0;
	ChangeFinishCaught(false);

	fcntl(listen_sock, F_SETFL, O_NONBLOCK); /* non-blocking accept*/

	while (1)
	{
		/* accept: wait for a connection request */
		if ((conn_sock[number_of_active_sock] = 
				accept(listen_sock, (struct sockaddr *) &clientaddr, &clientlen)) < 0)
		{
			//cout << "SERVER: ERROR while accepting incoming connection, probably, listening socket was closed" << endl;
			usleep(5000);
		}
		else
		{
			clientsIP[number_of_active_sock] = inet_ntoa(clientaddr.sin_addr);
			clientsPort[number_of_active_sock] = ntohs(clientaddr.sin_port);

			mess_to_print = "Starting a new thread to handle incoming connection from " + clientsIP[number_of_active_sock] + ":"
						+ to_string(clientsPort[number_of_active_sock]);
			PrintMessage(mess_to_print);

			fcntl(conn_sock[number_of_active_sock], F_SETFL, O_NONBLOCK); /* non-blocking read */			

			t[number_of_active_sock] = std::thread(echo, conn_sock[number_of_active_sock], clientsIP[number_of_active_sock],
														clientsPort[number_of_active_sock], number_of_active_sock);

			mess_to_print = "Is thread " + to_string(number_of_active_sock) + " joinable? "
								 + string((t[number_of_active_sock].joinable()) ? "YES" : "NO");
			PrintMessage(mess_to_print);

			number_of_active_sock++;
		}

		//PrintMessage("Awaiting incoming message...");

		if (finishCaught)
		{
			PrintMessage("Main thread is going to join client threads...");

			for(int i = 0; i < number_of_active_sock; i++)
			{

				mess_to_print = "Is thread " + to_string(i) + " joinable? "
								 + string((t[i].joinable()) ? "YES" : "NO");
				PrintMessage(mess_to_print);			 

				if (t[i].joinable())
				{
					close(conn_sock[i]);
					t[i].join();
					mess_to_print = "connection to " + clientsIP[i] + ":" + to_string(clientsPort[i])
									+ " was closed!";
					PrintMessage(mess_to_print);
				}
			}
			break;
		}
	}

	return 0;
}