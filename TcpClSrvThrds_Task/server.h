#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

#include <future>
#include <map>
#include <mutex>
#include <fstream>
#include <iostream>
#include <queue>

#define BUFF_SIZE 1024

using std::mutex;
using std::lock_guard;
using std::future;
using std::map;
using std::ofstream;
using std::async;
using std::endl;
using std::cerr;
using std::ios_base;
using std::queue;



class Server
{
public:
	
	int StartServer(int port);

	void AcceptLoop();

private:

	int CreateMasterSocket();
	void BuildServAddr(int port);
	int SetReuseAddr();
	int BindAddrToSocket();
	int Listen();
	int SetNonBlock();

	void OpenFstream();

	void HandleConnection(int slave_socket);

	int master_socket;
	struct sockaddr_in serveraddr;
	
	struct MutexedSockFutMap
	{
		mutex mut;
		map<int, future<void>> sock_to_fut_map;
	} mutxd_sockfutmap;

	struct MutexedFstream
	{
		mutex mut;
		ofstream log;
	} mutxd_fstream;

	struct MutexedClosingSocksQueue
	{
		mutex mut;
		queue<int> queue_of_closing_socks;
	} mutxd_queue;

	void AddSockToClosingSocksQueue(int slave_socket)
	{
		lock_guard<mutex> grd(mutxd_queue.mut);
		mutxd_queue.queue_of_closing_socks.push(slave_socket);
	}

	void EraseSockToFutRecord(int slave_socket)
	{
		lock_guard<mutex> grd(mutxd_sockfutmap.mut);
		mutxd_sockfutmap.sock_to_fut_map[slave_socket].get();
		mutxd_sockfutmap.sock_to_fut_map.erase(slave_socket);
		close(slave_socket);
	}

};

