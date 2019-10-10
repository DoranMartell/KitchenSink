#include "server.h"

int Server::CreateMasterSocket()
{
	master_socket = socket(AF_INET, SOCK_STREAM, 0);
	return master_socket;
}

void Server::BuildServAddr(int port)
{
	memset((char *) & serveraddr, '\0', sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(port);
}

int Server::SetReuseAddr()
{
	int enable = 1;
	return setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
}

int Server::BindAddrToSocket()
{
	return bind(master_socket, (struct sockaddr *) & serveraddr, sizeof(serveraddr));
}

int Server::SetNonBlock()
{
	return fcntl(master_socket, F_SETFL, O_NONBLOCK);
}

int Server::Listen()
{
	return listen(master_socket, SOMAXCONN);
}

void Server::OpenFstream()
{
	mutxd_fstream.log.open("log.txt", ios_base::out | ios_base::app);
}

void Server::AcceptLoop()
{
	int slave_socket;
	while (1)
	{
		slave_socket = accept(master_socket, NULL, NULL);
		if (slave_socket < 0)
		{	
			{
				lock_guard<mutex> grd(mutxd_queue.mut);

				while (!mutxd_queue.queue_of_closing_socks.empty())
				{
					int another_released_socket = mutxd_queue.queue_of_closing_socks.front();
					EraseSockToFutRecord(another_released_socket);
					mutxd_queue.queue_of_closing_socks.pop();
				}
			}

			sleep(1);
		}
		else
		{
			lock_guard<mutex> guard(mutxd_sockfutmap.mut);
			mutxd_sockfutmap.sock_to_fut_map[slave_socket] = 
						async([this, slave_socket](){Server::HandleConnection(slave_socket);});
		}
	}
}

void Server::HandleConnection(int slave_socket)
{
	char buff[BUFF_SIZE];
	memset(buff, '\0', sizeof(buff));
	int bytes_read;

	while ((bytes_read = read(slave_socket, buff, sizeof(buff))) != 0)
	{
		if (bytes_read == -1)
		{
			cerr << "Error reading" << endl;
			AddSockToClosingSocksQueue(slave_socket);
			return;
		}

		lock_guard<mutex> grd(mutxd_fstream.mut);
		if (!mutxd_fstream.log.is_open())
		{
			OpenFstream();
		}
		mutxd_fstream.log << buff;
		mutxd_fstream.log.flush();

		memset(buff, '\0', sizeof(buff));
		mutxd_fstream.log.close();
	}

	AddSockToClosingSocksQueue(slave_socket);
}

int Server::StartServer(int port)
{
	if (CreateMasterSocket() < 0)
	{
		cerr << "Error creating listening socket: " << strerror(errno) << endl;
		return -1;
	}

	BuildServAddr(port);

	if (SetReuseAddr() < 0)
	{
		cerr << "Error setting REUSEADDR option on listening socket: " << strerror(errno) << endl;
		return -2;
	}
	
	if (BindAddrToSocket() < 0)
	{
		cerr << "Error binding socket to address: " << strerror(errno) << endl;
		return -3;
	}

	if (SetNonBlock() < 0)
	{
		cerr << "Error switching listening socket into non-blocking mode: " << strerror(errno) << endl;
		return -4;
	}

	if (Listen() < 0)
	{
		cerr << "Error starting listening on socket: " << strerror(errno) << endl;
		return -5;
	}

	return 0;
}

int main(int argc, char const * argv[])
{	

	if (argc != 2)
	{
		cerr << "You should pass port number as the single command-line parameter!\n";
		exit(1);
	}

	Server serv;
	int why;
	if ((why = serv.StartServer(atoi(argv[1]))) < 0)
	{
		exit(why);
	}

	serv.AcceptLoop();


	return 0;
}