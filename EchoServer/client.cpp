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
#include <cstdlib>
#include <ctime>

#include <fcntl.h>

#define BUFSIZE 256

using namespace std;

struct sockaddr_in destination;
socklen_t destinationlen;

mutex mu;
bool finishCaught;

void ChangeFinishCaught(bool val)
{
	mu.lock();
	finishCaught = val;
	mu.unlock();
}

void PrintMessage(string msg)
{
	mu.lock();
	cout << "CLIENT: " << msg << endl;
	mu.unlock();
}

void BuildDestinationAddr(int port_num)
{
	/* building server's internet address to send messages there*/
	memset(&destination, '\0', sizeof(struct sockaddr_in));
	destination.sin_family = AF_INET;
	destination.sin_port = htons(port_num);
	inet_aton("127.0.0.1", &destination.sin_addr);
}

void AskDestinationPort()
{
	int p = 0;
	while (p <= 0 || p > 65000)
	{
		cout << "CLIENT: Please, enter a port number: ";
		cin >> p;
		cout << endl;
	}

	BuildDestinationAddr(p);
}

void SendMessages(int socket_number)
{
	long random_number;
	int n;
	char buf[BUFSIZE];
	memset(buf, 0, BUFSIZE);

	string buf_str;

	for (int i = 0; i < 500; ++i)
	{
		if (finishCaught)
		{
			memset(buf, '\0', BUFSIZE);
			return;
		}

		if (i != 499)
		{
			random_number = rand() % 150000;
			buf_str = string("Client socket " + to_string(socket_number) + " sent " + to_string(random_number));

			/* conversion of c++ string to char[] array, which is used in send() function */
			strcpy(buf, buf_str.c_str());
		}
		else
		{
			random_number = rand() % 3;

			switch (random_number)
			{
				case 0:
						strcpy(buf, "finish");
						break;
				case 1:
						strcpy(buf, "quit");
						break;
				case 2:
						strcpy(buf, "exit");
						break;
				default:
						break;
			}
		}

		/* sending a random message to server */
		if (send(socket_number, buf, strlen(buf), 0) < 0)
		{
			PrintMessage("ERROR sending a message");
			return;
		}

		PrintMessage(string(buf));

		memset(buf, '\0', sizeof(buf));

		if (i == 499)
		{
			ChangeFinishCaught(true);
			return;
		}

		fcntl(socket_number, F_SETFL, O_NONBLOCK); /* non-blocking recvfrom() */

		/* receiving response from server */
		while ((n = recvfrom(socket_number, buf, BUFSIZE, 0, (struct sockaddr *) &destination, &destinationlen)) < 0)
		{
			usleep(5000);
			if (finishCaught)
			{
				memset(buf, '\0', BUFSIZE);
				return;
			}
		}

		PrintMessage("Socket " + to_string(socket_number) + " recieved echo from server: " + string(buf));

		memset(buf, '\0', sizeof(buf));
	}
}


int main(int argc, char const *argv[])
{
	vector<int>	connection_sock(2);
	string mess_to_print;

	srand(time(NULL));

	finishCaught = false;
	vector<std::thread> t(2); /* array of threads - one thread per each connection */

	/* GOTO label as we might need to return there if connection failed to be established */
	ask_port:
	AskDestinationPort();

	for (int i = 0; i < 2; ++i)
	{	
		/* creating a socket to establish connection */
		connection_sock[i] = socket(AF_INET, SOCK_STREAM, 0);
	}

	for (int i = 0; i < 2; ++i)
	{
		if (connect(connection_sock[i], (struct sockaddr *) & destination, sizeof(struct sockaddr)) != 0 )
		{
			cout << "CLIENT: ERROR while establishing of connection to server!" << endl;
			for (int j = 0; j < 2; ++j)
			{
				close(connection_sock[j]);
			}
			goto ask_port;
		}

		t[i] = std::thread(SendMessages, connection_sock[i]);
		// mess_to_print = "Is thread " + to_string(i) + " joinable? "
		// 						 + string((t[i].joinable()) ? "YES" : "NO");
		// PrintMessage(mess_to_print);	
	}

	while (1)
	{
		if (finishCaught)
		{
			PrintMessage("Main thread is going to join connection threads...");

			for (int i = 0; i < 2; ++i)
			{	
				mess_to_print = "Is thread " + to_string(i) + " joinable? "
								 + string((t[i].joinable()) ? "YES" : "NO");
				PrintMessage(mess_to_print);			 

				if (t[i].joinable())
				{
					if (close(connection_sock[i]) != 0)
						PrintMessage("ERROR while closing connection!");

					t[i].join();
					PrintMessage("connection " + to_string(connection_sock[i]) + " was closed!");
				}
			}
			break;
		}
		else
			usleep(5000);
	}

	return 0;
}

