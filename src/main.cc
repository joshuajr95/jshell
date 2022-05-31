/*
 * Author: Joshua Jacobs-Rebhun
 *
 * This is the main shell program. All execution of commands, as well as initialization
 * is done here.
 */


#include <iostream>
#include <string>

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#include <builtin.h>
#include <builtin_list.h>
#include <hashtable.h>
#include <job.h>
#include <main.h>
#include <parse.h>
#include <signal_handlers.h>
#include <sighandler_list.h>


#define MAX_USERNAME_LENGTH 128
#define MAX_PATH_LENGTH 128
#define MAX_HOSTNAME_LENGTH 128


// various static shell variables
static char login_name[MAX_USERNAME_LENGTH];
static char host_name[MAX_HOSTNAME_LENGTH];
static int next_job_number = 1;


/*
 * Wrapper function for the getlogin and getlogin_r functions since there are compatibility
 * issues with getlogin_r for Mac OSX but not with Linux.
 */
int get_login_name(char *buf, size_t buf_size)
{
    int status;
#if defined(__APPLE__) || defined(__MACH__)
    buf = getlogin();

    if(buf == NULL)
        status = -1;
    else
        status = 0;
    

#else
    if(buf_size <= MAX_USERNAME_SIZE)
    {
        status = getlogin_r(buf, buf_size);
    }
    else
    {
        status = -1;
    }
#endif

    return status;
}


std::string make_prompt()
{
    char current_path[MAX_PATH_LENGTH];
    getcwd(current_path, MAX_PATH_LENGTH);

    std::vector<std::string> tok_path = tokenize(current_path, "/");
    std::string current_dir = tok_path[tok_path.size()-1];
    std::string login(login_name);
    std::string hostname(host_name);
    std::string prompt = login + std::string("@") + hostname + std::string(" ") + current_dir + std::string(" % ");

    return prompt;
}


void initialize_builtin_table()
{
    for(int i = 0; i < num_builtins; i++)
    {
        builtin_table.insert(builtin_commands_list[i], builtin_list[i]);
    }
}

void initialize_sighandler_table()
{
    for(int i = 0; i < num_sighandlers; i++)
    {
        sighandler_table.insert(signum_list[i], signal_handler_list[i]);
    }
}



/**********************
 * Main shell Program *
 **********************/

int main(int argc, char *argv[])
{

    // initialize login name
    if(get_login_name(login_name, MAX_USERNAME_LENGTH) != 0)
    {
        std::cout << "Could not login. Exiting..." << std::endl;
        return -1;
    }

    if(gethostname(host_name, MAX_HOSTNAME_LENGTH) != 0)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    // initialize the various tables for shell functionality
    initialize_builtin_table();
    initialize_sighandler_table();

    while(true)
    {
        std::string prompt = make_prompt();

        std::cout << prompt;

        std::string command_input;
        std::cin >> command_input;

        Job current_job;

        try
        {
            current_job = getJob(command_input);
        }
        catch(const std::runtime_error& e)
        {
            std::cout << "Parse error. Please enter correct syntax." << std::endl;
            continue;
        }

        // empty command
        if(current_job.getNumCommands() == 0)
        {
            continue;
        }

        else if(current_job.getNumCommands() == 1)
        {
            Command command = current_job.getCommands()[0];
            if(builtin_table.contains(command.getTokenArray()[0]))
            {
                builtin_t command_function = builtin_table[command.getTokenArray()[0]];

                if(command_function(command.getNumTokens(), &command.getTokenArray()[0]) != 0)
                {
                    std::cout << "Error executing builtin command..." << std::endl;
                }

                continue;
            }
        }

        else
        {
            // plumbing
            int fds[current_job.getNumCommands()-1][2];
            pid_t pids[current_job.getNumCommands()];

            for(int i = 0; i < current_job.getNumCommands(); i++)
            {
                Command current_command = current_job.getCommands()[i];

                // make pipes
                if(i < current_job.getNumCommands()-1)
                    pipe(fds[i]);

                pids[i] = fork();

                // error in forking
                if(pids[i] < 0)
                {
                    std::cout << strerror(errno) << std::endl;
                    break;
                }

                // child process
                else if(pids[i] == 0)
                {
                    
                    // use dup2 to redirect stdout and stdin into and out of pipes
                    if(i > 0 && i < current_job.getNumCommands()-1)
                    {
                        dup2(fds[i-1][0], STDIN_FILENO);
                        dup2(fds[i][1], STDOUT_FILENO);
                        close(fds[i][1]);
                        close(fds[i-1][0]);
                    }
                    else if(i == 0)
                    {
                        close(fds[i][0]);
                        dup2(fds[i][1], STDOUT_FILENO);
                        close(fds[i][1]);
                    }
                    else if(i == current_job.getNumCommands()-1)
                    {
                        close(fds[i-1][1]);
                        dup2(fds[i-1][0], STDIN_FILENO);
                        close(fds[i-1][0]);
                    }
                    

                    // redirect input, output, and error
                    if(current_command.isInputRedirected() && (i == 0))
                    {
                        int redirected_input = open(current_command.getInputFiles()[0].c_str(), O_RDONLY);
                        dup2(redirected_input, STDIN_FILENO);
                        close(redirected_input);
                    }

                    if(current_command.isOutputRedirected() && (i == current_job.getNumCommands()-1))
                    {
                        int redirected_output = open(current_command.getOutputFiles()[0].c_str(), O_WRONLY);
                        dup2(redirected_output, STDOUT_FILENO);
                        close(redirected_output);
                    }

                    if(current_command.isErrorRedirected())
                    {
                        int redirected_error = open(current_command.getErrorFiles()[0].c_str(), O_WRONLY);
                        dup2(redirected_error, STDOUT_FILENO);
                        close(redirected_error);
                    }


                    int num_args = current_command.getTokenArray().size();
                    char *args[num_args+1];

                    for(int j = 0; j < num_args; j++)
                    {
                        strcpy(args[j], current_command.getTokenArray()[j].c_str());
                    }
                    args[num_args] = NULL;

                    execvp(args[0], args);
                }

                // parent process
                else
                {
                    if(i > 0 && i < current_job.getNumCommands()-1)
                    {
                        close(fds[i][1]);
                        close(fds[i-1][0]);
                    }
                    else if(i == 0)
                    {
                        close(fds[i][1]);
                    }
                    else if(i == current_job.getNumCommands()-1)
                    {
                        close(fds[i-1][0]);
                    }

                    if(i == current_job.getNumCommands()-1 && current_job.isBackground())
                    {
                        job_table.insert(next_job_number, current_job);
                        break;
                    }
                }
            }

            // wait on child processes if job is not run in the background
            if(!current_job.isBackground())
            {
                for(int i = 0; i < current_job.getNumCommands(); i++)
                {
                    int status;
                    waitpid(pids[i], &status, 0);
                }
            }
            
        }
    }

    

    return 0;
}
