#include "exit_handle.hpp"
#include "class/request.hpp"

int exit_code;

#define CGI_FILES_PATH "memory/"

void signalHandler(int signum)
{
	std::cout << std::endl
			  << "Goodbye! That was cool to have you :)" << std::endl;

	Request interface;
	interface.delete_all_files();
	exit_code = signum;
}

bool shouldQuit()
{
    return exit_code != 0;
}

void handleExit()
{
	exit_code = 0;
	signal(SIGINT, signalHandler);
	signal(SIGPIPE, SIG_IGN);
}
