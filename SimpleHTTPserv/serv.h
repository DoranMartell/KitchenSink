#include <arpa/inet.h>
#include <stdlib.h>  /* atoi(), exit() */
#include <sys/types.h>
#include <sys/socket.h>
#include <syslog.h> /* openlog(), syslog() */
#include <stdio.h> /* sprintf() */
#include <fcntl.h> /* O_RDONLY, O_WRONLY, O_NONBLOCK */
#include <string.h> /* memset() */
#include <sys/epoll.h>
#include <pthread.h>
#include <errno.h>

#define ERR_MSG_SIZE 256
#define MAX_EVENTS 32
#define REQUEST_SIZE 4096
#define PATH_SIZE 1024
#define METHOD_SIZE 8
#define URI_SIZE 128
#define VERSION_SIZE 9
#define BUFFER_SIZE 4096


void StartServer(char * ip_addr_str, char * port_str, char * directory_path);