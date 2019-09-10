/* Final task of Mail.Ru course on Stepik:
	HTTP 1.0 server which is able to reply on GET requests. It was required to be multithreaded.
	Start server with flags: -h <ip_addr> -p <port_number> -d <home_directory_of_server>,
	it becomes a daemon and logs its errors and some info in syslog.
	Response type (200/404) depends on existence of <home_directory_of_server> + <path from GET-request> file.
*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "serv.h" // StartServer() function

#define ADDR_SIZE 15// xxx.xxx.xxx.xxx one char - one byte = (4 * 3 + 3) * sizeof(char)
#define PORT_SIZE 5 // portnumber range from 0 to 65535

int main(int argc, char * argv[])
{
	int opt;

	char ip_addr_str[ADDR_SIZE];
	char port_str[PORT_SIZE];
	char directory_path[PATH_SIZE];
	opterr = 0; // value 0 to allow getting invalid options

	if (argc == 1)
	{
		printf("Usage: <program_name> -h <ip_addr> -p <port_number> -d <directory>\n");
		exit(1);
	}

				/* if flag has parameters after it, we put colon sign in optstring of getopt() function */
	while ((opt = getopt(argc, argv, "h:d:p:")) != -1)
	{
		if (opt != '?')
		{								/* optarg is a global variable created by getopt() call */
			printf("Option %c included with value: %s\n", opt, optarg);
		}

		if (opt == 'h')
		{
			memset(ip_addr_str, '\0', sizeof(ip_addr_str));
			strcpy(ip_addr_str, optarg);
			printf("Ip addr: %s\n", ip_addr_str);
		}
		else if (opt == 'p')
		{
			memset(port_str, '\0', sizeof(port_str));
			strcpy(port_str, optarg);
			printf("Port number: %s\n", port_str);
		}
		else if (opt == 'd')
		{
			memset(directory_path, '\0', sizeof(directory_path));
			strcpy(directory_path, optarg);
			printf("Path to directory with server: %s\n", directory_path);
		}
		else if (opt == '?')
		{
			printf("Sorry, only flags -h <ip_addr> -p <port_number> -d <directory> are supported\n");
			exit(1);
		}
	}

	StartServer(ip_addr_str, port_str, directory_path);

	return 0;
}