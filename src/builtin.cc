/*
 * File: builtin.cc
 * Author: Joshua Jacobs-Rebhun
 * Date: May 22, 2022
 * 
 * This file implements all of the functions that are called by the builtin commands.
 */


#include <iostream>
#include <string>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>


#include <main.h>
#include <parse.h>

#define MAX_PATHNAME_LENGTH 128


// since the system call interfaces take C-style strings or string
// arrays as arguments, it is necessary to convert between the C++-style
// std::strings and the C-style char* strings
static void cpp_args_to_c_args(char *c_argv[], int argc, std::string argv[])
{
    for(int i = 0; i < argc; i++)
    {
        const char *temp_str = argv[i].c_str();
        c_argv[i] = (char*) malloc(sizeof(char) * (strlen(temp_str) + 1));
        strcpy(c_argv[i], temp_str);
    }
}


// free all of the args memory. Must be used after converting C++ style
// args to C style args since this is done using malloc and failure to
// free the memory will cause a memory leak
static void free_all_args(int argc, char *argv[])
{
    for(int i = 0; i < argc; i++)
    {
        free(argv[i]);
    }
}



/****************************************
 * Builtins inherited from Bourne shell *
 ****************************************/

BUILTIN_TABLE int do_builtin_cd(int argc, std::string argv[])
{
    if(argc > 2 || argc < 0)
    {
        std::cout << "Incorrect number of arguments to cd" << std::endl;
        return -1;
    }

    char *args[argc];
    cpp_args_to_c_args(args, argc, argv);
    char *arg;
    
    if(argc == 1)
    {
        arg = (char*) "~";
    }
    else
    {
        arg = args[1];
    }

    int retval = chdir(arg);

    free_all_args(argc, args);

    if(retval == 0)
    {
        return 0;
    }

    std::cout << strerror(errno) << std::endl;
    
    return -1;
}


BUILTIN_TABLE int do_builtin_dot(int argc, std::string argv[])
{
    if(argc == 0)
    {
        std::cout << "Incorrect number of arguments to ./" << std::endl;
        return -1;
    }

    // convert to C style argv
    char *args[argc];
    cpp_args_to_c_args(args, argc, argv);

    pid_t pid = fork();
    int status;

    // error
    if(pid == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    // child process
    else if(pid == 0)
    {
        execvp(args[0], args);
        std::cout << strerror(errno) << std::endl;
        _exit(1);
    }

    else
    {
        waitpid(pid, &status, 0);
    }

    free_all_args(argc, args);

    return 0;
}


BUILTIN_TABLE int do_builtin_exit(int argc, std::string argv[])
{
    if(argc > 1 || argc < 0)
    {
        std::cout << "Incorrect number of arguments to exit" << std::endl;
        return -1;
    }

    if(argc == 1)
    {
        const char *arg = argv[0].c_str();
        _exit(atoi(arg));
    }

    _exit(0);
}


BUILTIN_TABLE int do_builtin_export(int argc, std::string argv[])
{
    if(argc != 2)
    {
        std::cout << "Incorrect format to export. Correct usage: export VARNAME=VALUE" << std::endl;
        return -1;
    }

    std::vector<std::string> tokens = tokenize(argv[1], "=");

    if(tokens.size() != 2)
    {
        std::cout << "Incorrect format to export. Correct usage: export VARNAME=VALUE" << std::endl;
        return -1;
    }

    if(setenv(tokens[0].c_str(), tokens[1].c_str(), 1) == -1)
    {
        std::cout << strerror(errno) << std::endl;
    }

    return 0;
}


BUILTIN_TABLE int do_builtin_pwd(int argc, std::string argv[])
{
    if(argc != 1)
    {
        std::cout << "Incorrect number of arguments to pwd" << std::endl;
        return -1;
    }

    char buf[MAX_PATHNAME_LENGTH];
    if (getcwd(buf, MAX_PATHNAME_LENGTH) == NULL)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    std::cout << buf << std::endl;
    return 0;
}


BUILTIN_TABLE int do_builtin_umask(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}

BUILTIN_TABLE int do_builtin_unset(int argc, std::string argv[])
{
    if(argc != 2)
    {
        std::cout << "Incorrect format for unset. Correct usage: unset VARNAME" << std::endl;
        return -1;
    }

    if(unsetenv(argv[1].c_str()) == -1)
    {
        std::cout << strerror(errno) << std::endl;
        return -1;
    }

    return 0;
}




/**********************************************
 * Builtins inherited from Bourne again shell *
 **********************************************/

BUILTIN_TABLE int do_builtin_alias(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}


BUILTIN_TABLE int do_builtin_echo(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}


BUILTIN_TABLE int do_builtin_kill(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}


BUILTIN_TABLE int do_builtin_source(int argc, std::string argv[])
{
    int retval = do_builtin_dot(argc, argv);

    return retval;
}


BUILTIN_TABLE int do_builtin_unalias(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}



/************************
 * Job control Builtins *
 ************************/

BUILTIN_TABLE int do_builtin_bg(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}


BUILTIN_TABLE int do_builtin_fg(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}


BUILTIN_TABLE int do_builtin_jobs(int argc, std::string argv[])
{
    std::cout << "Not implemented..." << std::endl;
    return -1;
}

