#include "serv.h"

struct sockaddr_in BuildSockAddr(const char * addr_string, const char * port_string)
{
	struct sockaddr_in result;

	if (inet_aton(addr_string, &(result.sin_addr)) == 0)
	{
		syslog(LOG_ERR, "Invalid IP address!");
		exit(1);
	}

	result.sin_family = AF_INET;
	result.sin_port = htons(atoi(port_string));

	return result;
}

void LogErrAndClose(const char * message, int sock_dscrpt)
{
	syslog(LOG_ERR, "%s\n", message);

	if (sock_dscrpt != -1)
		close(sock_dscrpt);

	closelog();
	exit(1);
}

int set_nonblock(int fd)
{
	int flags;

	if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
		flags = 0;

	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

/* Special structure for everything we need to process a client request and to reply back */
struct arguments
{
	int socket_dscrptr;
	char * received_request;
	int received_request_lngth;
	char directory[PATH_SIZE];

};

void * ParseAndReply(void * args);
void ParseAnotherEntity(char * , unsigned long , unsigned long * , char * , unsigned long , unsigned long * );


void StartServer(char * ip_addr_str, char * port_str, char * directory_path)
{

	/* Daemonizing */
	close(0); /* daemon should not use standard streams... */
	close(1);
	close(2);
	/* ...but it is better to keep them opened anyway in case some function */
	open("/dev/null", O_RDONLY); /* reads from there (descriptor 0) */
	open("/dev/null", O_WRONLY); /* or writes in there (descriptor 1) */
	open("/dev/null", O_WRONLY); /* or writes about errors in there (descriptor 2) */

	chdir("/"); /* make sure if we remove/rename some directory, daemon will certainly not be affected by it */

	/* we do it to be sure that setsid() succeeds (as the process could have already been a group/session leader) */
	if (fork() > 0)
	{
		exit(0);
	}
	setsid(); /* separate session for the daemon, save us from SIGHUPs
					(daemon shouldn't have 'controlling terminal' and shouldn't receive such signals when this terminal is closed) */

	/* after making another session, daemon becomes its leader and can get a controlling terminal */
	if (fork() > 0) /* after fork() the child process will not be a leader */
	{
		exit(0);
	}

	/* Daemon is going to write to a system log*/
	char message[ERR_MSG_SIZE];
	memset(message, '\0', sizeof(message));
	openlog("Final Task of Mail.Ru course", 0, LOG_USER);
	syslog(LOG_INFO, "Created daemon process with server of Final Task of Mail.Ru course with PID %d\n", getpid());

	/* Create listening socket, build server addr, bind them together and start listening for client connections */
	struct sockaddr_in server = BuildSockAddr(ip_addr_str, port_str);

	int master_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
	if (master_socket_descriptor == -1)
	{
		sprintf(message, "Error creating master (listening) socket!");
		LogErrAndClose(message, -1);
		exit(1);
	}

	int enable = 1; /* 1 for enabling option SO_REUSEADDR (no need to wait on this port after server was shut down) */
	if (setsockopt(master_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0)
	{
		sprintf(message, "Could not set SO_REUSEADDR option!");
		LogErrAndClose(message, master_socket_descriptor);
		exit(1);
	}

	if (bind(master_socket_descriptor, (struct sockaddr *) & server, sizeof(server)) == -1)
	{
		sprintf(message, "Error while binding server address to the master socket!");
		LogErrAndClose(message, master_socket_descriptor);
		exit(1);
	}

	set_nonblock(master_socket_descriptor); /* for non-blocking accept() */

	if (listen(master_socket_descriptor, SOMAXCONN) == -1)
	{
		sprintf(message, "Error marking master socket as passive listening to incoming connections!");
		LogErrAndClose(message, master_socket_descriptor);
		exit(1);
	}

	/* Create epoll instance */
	int epoll_dscrptr = epoll_create1(0);
	if (epoll_dscrptr == -1)
	{
		sprintf(message, "Error creating epoll file descriptor!");
		LogErrAndClose(message, master_socket_descriptor);
		exit(1);
	}

	struct epoll_event event; /* only two fields:
								 'uint32_t' for 'events' and 'union (with fd being the most important field)' for 'data' */
	event.data.fd = master_socket_descriptor;
	event.events = EPOLLIN; /* The associated 'master_socket_descriptor' is available for read() operations */
	
	/* Register (EPOLL_CTL_ADD) 'master_socket_descriptor' on the epoll instance referred to by 'epoll_dscrptr' 
				and associate the event 'event' with the internal file linked to 'master_socket_descriptor'. */
	epoll_ctl(epoll_dscrptr, EPOLL_CTL_ADD, master_socket_descriptor, & event);

	while (1)
	{
		struct epoll_event events[MAX_EVENTS];

		/* epoll_wait() call is blocked until some file descriptors (n) are ready for reading (as we set only EPOLLIN) */
		int n = epoll_wait(epoll_dscrptr, events, MAX_EVENTS, -1); /* -1 means infinite timeout */

		int i;
		for (i = 0; i < n; ++i)
		{
			/* Once new connection request arrives - it is percieved as an event on listening socket */ 
			if (events[i].data.fd == master_socket_descriptor)
			{
				int slave_socket_descriptor = accept(master_socket_descriptor, 0 , 0);
				set_nonblock(slave_socket_descriptor); /* for non-blocking recv() */
				
				/* Watch for ready-for-reading events happen on client socket and add it to epoll instance */
				struct epoll_event event;
				event.data.fd = slave_socket_descriptor;
				event.events = EPOLLIN; /* The associated 'slave_socket_descriptor' is available for read() operations */

				/* Register (EPOLL_CTL_ADD) 'slave_socket_descriptor' on the epoll instance referred to by 'epoll_dscrptr' 
					and associate the event 'event' with the internal file linked to 'slave_socket_descriptor'. */
				epoll_ctl(epoll_dscrptr, EPOLL_CTL_ADD, slave_socket_descriptor, & event);
			}
			else /* we get here if an event occurred at some of 'client-connection' sockets */
			{
				char request[REQUEST_SIZE];
				int recv_result = recv(events[i].data.fd, request, REQUEST_SIZE, MSG_NOSIGNAL);

				if (recv_result == 0 && errno != EAGAIN)
				{
					shutdown(events[i].data.fd, SHUT_RDWR);
					close(events[i].data.fd);
				}
				else if (recv_result > 0)
				{
					/* Special structure for everything we need to process a client request and to reply back */
					struct arguments args;

					args.socket_dscrptr = events[i].data.fd;
					args.received_request = (char *) malloc(sizeof(request));
					strcpy(args.received_request, request);
					args.received_request_lngth = recv_result;
					memset(args.directory, '\0', sizeof(args.directory));
					strcpy(args.directory, directory_path);

					pthread_t thr;
					if (pthread_create(&thr, NULL, ParseAndReply, & args) != 0)
					{
						sprintf(message, "Error creating thread to handle client's request!");
						LogErrAndClose(message, -1);
						free(args.received_request);
						continue;
					}

					if (pthread_detach(thr) != 0)
					{
						sprintf(message, "Error detaching thread to handle client's request!");
						LogErrAndClose(message, -1);
						free(args.received_request);
						continue;
					}
				}
			}
		}
	}

	close(master_socket_descriptor);
	
}



void * ParseAndReply(void * args)
{
	struct arguments * args_ptr = (struct arguments *) args;

	char * response_message;

	char * http_request = args_ptr->received_request;
	unsigned long http_request_size = args_ptr->received_request_lngth;
	unsigned long request_current_position = 0;

	char * method = (char *) malloc(METHOD_SIZE);
	unsigned long method_actual_size = 0;
	ParseAnotherEntity(http_request, http_request_size, &request_current_position, method, METHOD_SIZE, &method_actual_size);

	syslog(LOG_INFO, "Method %s\n", method);
	syslog(LOG_INFO, "Method actual size is %ld\n", method_actual_size);
	syslog(LOG_INFO, "Current position in http request is %ld\n", request_current_position);

	if (method_actual_size == 0)
	{
		syslog(LOG_ERR, "METHOD cannot be empty! Request is incorrect!");
		free(method);
		return NULL;
	}

	char * uri = (char *) malloc(URI_SIZE);
	unsigned long uri_actual_size = 0;
	ParseAnotherEntity(http_request, http_request_size, &request_current_position, uri, URI_SIZE, &uri_actual_size);

	syslog(LOG_INFO, "URI (Uniform Resourse Identifier) %s\n", uri);
	syslog(LOG_INFO, "URI actual size is %ld\n", uri_actual_size);
	syslog(LOG_INFO, "Current position in http request is %ld\n", request_current_position);

	if (uri_actual_size == 0)
	{
		syslog(LOG_ERR, "URI cannot be empty! Request is incorrect!");
		free(method);
		free(uri);
		return NULL;
	}

	char * protocol_version = (char *) malloc(VERSION_SIZE);
	unsigned long protocol_version_actual_size = 0;
	ParseAnotherEntity(http_request, http_request_size, &request_current_position, protocol_version, VERSION_SIZE, &protocol_version_actual_size);

	syslog(LOG_INFO, "Protocol Version %s\n", protocol_version);
	syslog(LOG_INFO, "Protocol Version actual size is %ld\n", protocol_version_actual_size);
	syslog(LOG_INFO, "Current position in http request is %ld\n", request_current_position);

	if (protocol_version_actual_size == 0)
	{
		syslog(LOG_ERR, "Protocol VERSION cannot be empty! Request is incorrect!");
		free(method);
		free(uri);
		free(protocol_version);
		return NULL;
	}

	char * full_path = (char *) malloc(uri_actual_size + sizeof(args_ptr->directory));
	memset(full_path, '\0', sizeof(full_path));
	strcat(full_path, args_ptr->directory);
	strcat(full_path, uri);

	syslog(LOG_INFO, "Full path to the document we are looking for %s\n", full_path);

	int fd = open(full_path, O_RDONLY, 0666);
	char content_type_str[] = "Content-Type: text/html\r\n";
	char content_length_str[] = "Content-length: ";
	if (fd == -1)
	{
		syslog(LOG_ERR, "Error opening file specified as full path with errno %d", errno);
		syslog(LOG_ERR, "%s", strerror(errno));
		syslog(LOG_INFO, "Going to send response with 404");

		char not_found_str[] = " 404 Not Found\r\n";
		int response_message_size = protocol_version_actual_size + sizeof(content_type_str) + sizeof(content_length_str) + sizeof(not_found_str) + 2 * 2;

		response_message = (char *) malloc(response_message_size);
		memset(response_message, '\0', response_message_size);
		
		/* I know it doesn't look very neat, but you can see particles of response message separately */
		strcat(response_message, protocol_version);
		strcat(response_message, not_found_str);
		strcat(response_message, content_type_str);
		strcat(response_message, content_length_str);
		strcat(response_message, "0\n");
		strcat(response_message, "\r\n");

		syslog(LOG_INFO, "Response message for client %s", response_message);

		write(args_ptr->socket_dscrptr, response_message, response_message_size);
	}
	else
	{
		syslog(LOG_INFO, "File %s opened, going to send its contents in '200'-type message", full_path);
		char * buffer = (char *) malloc(BUFFER_SIZE);
		memset(buffer, '\0', BUFFER_SIZE);

		int bytes_read_from_file = read(fd, buffer, BUFFER_SIZE);

		syslog(LOG_INFO, "%d bytes read from file %s", bytes_read_from_file, full_path);

		if (bytes_read_from_file == -1)
		{
			syslog(LOG_ERR, "Error reading from file %s", full_path);
		}
		else
		{
			char ok_str[] = " 200 OK\r\n";

			char bytes_read_str[10];
			memset(bytes_read_str, '\0', sizeof(bytes_read_str));
			sprintf(bytes_read_str, "%d", bytes_read_from_file);

			int response_message_size = protocol_version_actual_size + sizeof(ok_str) + sizeof(content_type_str) + sizeof(content_length_str) + sizeof(bytes_read_str) + bytes_read_from_file + 2 * 2/* * 4*/ + 1;
			response_message = (char *) malloc(response_message_size);
			memset(response_message, '\0', response_message_size);

			/* I know it doesn't look very neat, but you can see of what response message actually consists of */
			strcat(response_message, protocol_version);
			strcat(response_message, ok_str);
			strcat(response_message, content_type_str);
			strcat(response_message, content_length_str);
			strcat(response_message, bytes_read_str);
			strcat(response_message, "\r\n");
			strcat(response_message, "\r\n");
			strcat(response_message, buffer);
			strcat(response_message, "\n");

			syslog(LOG_INFO, "Response message for client %s", response_message);

			write(args_ptr->socket_dscrptr, response_message, response_message_size);
		}

		close(fd);
		free(buffer);
	}
	
	/* Left after testing: as if it was an echo server */
	/* write(args_ptr->socket_dscrptr, args_ptr->received_request, args_ptr->received_request_lngth); */

	free(response_message);
	free(full_path);
	free(method);
	free(uri);
	free(protocol_version);

	return NULL;
}

void ParseAnotherEntity(char * request, unsigned long request_size,
							 unsigned long * current_position, char * result, unsigned long expected_size, unsigned long * actual_size)
{

	syslog(LOG_INFO, "ParseAnotherEntity() expected size %ld", expected_size);
	memset(result, '\0', expected_size * sizeof(char));

	*actual_size = 0;
	int uri_parameter_start_ignoring = 0; /* once this value becomes equal to 1, we are not writing contents into result */
	char c;
	while ((c = request[(*current_position)++]) != ' '
			&& c != EOF
			&& c != '\r'
			&& c != '\0'
			&& c != '\n'
			&& (*actual_size) < expected_size
			&& (*current_position) < request_size)
	{
		if (c == '?') /* ignoring everything what starts with question mark: '/index.html?parameter=stuff' */
		{
			uri_parameter_start_ignoring = 1;
		}
			
		if (!uri_parameter_start_ignoring)	
		{
			result[(*actual_size)++] = c;
		}
		/* syslog(LOG_INFO, "ParseAnotherEntity() current state of result %s", result); */
	}

	/* syslog(LOG_INFO, "ParseAnotherEntity() actual size %ld", *actual_size);
	syslog(LOG_INFO, "ParseAnotherEntity() result %s", result); */

}