/* Takes a sequence of commands like "ls -l | sort -k9 | head -3" as input (from stdin)
	and writes result of execution of whole sequence into result.out file */

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <string.h>
#include <fcntl.h> 
#include <sys/wait.h> 

using namespace std;

#define SIZE 1024
#define NUM_OF_ARGS 50

struct name_and_args
{
	char name[SIZE];
	int argc;
	char * argv[NUM_OF_ARGS]; // each new argument allocated in heap
};

vector<string> ParseCommands(const string & input);
vector<name_and_args> ParseNamesAndArgs(vector<string> & full_commands);
void ExecuteWithPipe(vector<name_and_args> & names_and_args);

void TrimBack(string & s);
void TrimFront(string & s);

int main(int argc, char const * argv[])
{
	string what_to_do;

	// cout << "Please, enter a sequence of commands (each new command uses output of the previous as its input) separated with pipe sign:\n";
	getline(cin, what_to_do, '\n');
	// what_to_do = "ls -l | sort";

	// cout << "String received: " << what_to_do << endl;

	vector<string> separated = ParseCommands(what_to_do);

	// for each command - vector of program names and its command-line arguments
	vector<name_and_args> names_and_args = ParseNamesAndArgs(separated);

	ExecuteWithPipe(names_and_args);

	return 0;
}

void TrimBack(string & s)
{
	while (s.back() == ' ')
	{
		s.erase(s.size() - 1, 1);
	}
}

void TrimFront(string & s)
{
	int pos_first_not_space = s.find_first_not_of(" ");
	s = s.substr(pos_first_not_space);
}

vector<string> ParseCommands(const string & input)
{
	vector<string> commands_with_flags;

	size_t curr_start = 0; // starting position of parsing
	size_t where = 0;
	while ((where = input.find('|', curr_start)) != string::npos)
	{		
		string curr_command = input.substr(curr_start, where - curr_start);

		TrimFront(curr_command);
		TrimBack(curr_command);

		commands_with_flags.push_back(curr_command);
		curr_start = ++where;
	}

	string curr_command = input.substr(curr_start);

	TrimFront(curr_command);
	TrimBack(curr_command);

	commands_with_flags.push_back(curr_command);

	return commands_with_flags;

}

vector<name_and_args> ParseNamesAndArgs(vector<string> & full_commands)
{
	vector<name_and_args> names_and_args(full_commands.size());

	for (size_t i = 0; i < full_commands.size(); ++i)
	{
		int curr_start = 0;
		int where = 0;
		
		memset(names_and_args[i].name, '\0', sizeof(names_and_args[i].name));
		names_and_args[i].argc = 0; // haven't found another command's arguments yet

		while ((where = full_commands[i].find(' ', curr_start)) != string::npos)
		{
			if (names_and_args[i].name[0] == '\0')
			{
				strcpy(names_and_args[i].name, full_commands[i].substr(curr_start, where - curr_start).c_str());
			}

			names_and_args[i].argv[names_and_args[i].argc] = (char *) malloc(sizeof(char) * (where - curr_start));
			strcpy(names_and_args[i].argv[names_and_args[i].argc], full_commands[i].substr(curr_start, where - curr_start).c_str());
			names_and_args[i].argc++;
			
			curr_start = ++where;
		}
		
		if (names_and_args[i].name[0] == '\0')
		{
			strcpy(names_and_args[i].name, full_commands[i].substr(curr_start).c_str());
		}

		names_and_args[i].argv[names_and_args[i].argc] = (char *) malloc(sizeof(char) * (full_commands[i].size() - curr_start));
		strcpy(names_and_args[i].argv[names_and_args[i].argc], full_commands[i].substr(curr_start).c_str());
		names_and_args[i].argc++;

		/* No need for this last NULL argument */
		// names_and_args[i].argv[names_and_args[i].argc] = (char *) malloc(sizeof(char));
		// names_and_args[i].argv[names_and_args[i].argc][0] = '\0';
		// names_and_args[i].argc++;

	}

	return names_and_args;

}

void ExecuteWithPipe(vector<name_and_args> & names_and_args)
{

	string result;

	int fd;
	fd = open("result.out", O_RDWR | O_CREAT | O_TRUNC, 0666);  
	// FILE * fd = fopen("result.out", "rw");        

	char buff[1024];
	int how_much = 0;
	memset(buff, '\0', 1024);

	for (int i = 0; i < names_and_args.size(); ++i)
	{

		int parent_out_to_child_in[2];
		pipe(parent_out_to_child_in);

		int child_out_to_parent_in[2];
		pipe(child_out_to_parent_in);

		if (fork() == 0)
		{

			dup2(parent_out_to_child_in[0], STDIN_FILENO); // child reads from this pipe (where parent has written)
			close(parent_out_to_child_in[0]); // already associated with STDIN
			close(parent_out_to_child_in[1]); // child cannot write to it

			// if (i != 0)
			// {
				// how_much = read(STDIN_FILENO/*child_out_to_parent_in[0]*/, buff, 1024);
				// fprintf(stderr, "\n\n%d bytes read in CHILD!!: %s\n", how_much, buff);
			// }


			dup2(child_out_to_parent_in[1], STDOUT_FILENO); // child writes in here for parent to read
			close(child_out_to_parent_in[1]); // already associated with STDOUT
			close(child_out_to_parent_in[0]); // child cannot read from it (only parent can do it)

			int res = execvp(names_and_args[i].name, names_and_args[i].argv);
			
			if (res < 0)
			{
				fprintf(stderr, "Something went wrong!\n");
				exit(1);
			}

			exit(0);
		}

		/* Child reads what parent has written - therefore, parent cannot read from this pipe */
		// dup2(parent_out_to_child_in[1], STDOUT_FILENO); // we won't associate stdout of parent with this pipe, let the descriptor be as it is


		// previously it was done before fork(), making all of those descriptors closed in child as well!!
		// close(parent_out_to_child_in[1]); // we haven't associated this descriptor with stdout of parent - therefore, no need to close it
		close(parent_out_to_child_in[0]);

		/* Child writes for parent to read - therefore, parent cannot write to this pipe */
		dup2(child_out_to_parent_in[0], STDIN_FILENO);
		close(child_out_to_parent_in[1]);
		close(child_out_to_parent_in[0]);

		if (i != 0)
		{
			// fprintf(stderr, "Writing the following into parent-out-child-in descriptor:\n%s\n", buff);
			write(/*STDOUT_FILENO*/ parent_out_to_child_in[1], buff, how_much);
			memset(buff, '\0', 1024);
			close(/*STDOUT_FILENO*/parent_out_to_child_in[1]); /* !!! THIS IS NEEDED TO BE DONE TO INDICATE THAT NOTHING IS GOING TO BE READ ANYMORE!
																OTHERWISE, THE CHILD PROCESS WILL HANG ON READ() FOREVER, NOT KNOWING WHETHER WRITING IS FINISHED OR NOT */
			/* we are not associating parent_out end of pipe with stdout specifically for not closing it every iteration */

		}

		wait(NULL);

		how_much = read(STDIN_FILENO/*child_out_to_parent_in[0]*/, buff, 1024);

		for (int j = 0; j < names_and_args[i].argc; ++j)
		{
			// fprintf(stderr, "Free memory previously allocated for %d argument of %d command\n", j, i);
			free(names_and_args[i].argv[j]);
		}

	}

	write(fd, buff, how_much);
	// fprintf(fd, "%s", buff);

	close(fd);	

}
