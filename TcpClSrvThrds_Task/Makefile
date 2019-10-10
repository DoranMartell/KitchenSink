all : client server

client : client.cpp client.h
	g++ -o Client client.cpp

server : server.cpp server.h
	g++ -o Server server.cpp -pthread -std=c++11

clean:
	rm -f Client Server log.txt