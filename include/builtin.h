#ifndef BUILTIN_H
#define BUILTIN_H

#include <string>


// macro is used to mark each built-in function for the built-in table
// header auto-generation shell script
#define BUILTIN_TABLE

// this defines the builtin function pointer type that is used in the
// built-in table to access these functions
typedef int (*Builtin)(int argc, std::string argv[]);



/***********************************************************************
 * This section defines the functions that execute the builtin commands
************************************************************************/


// functions are in the format: 'do_builtin' followed by the name of the command


// the following functions execute Bourne shell (sh) specific builtins
BUILTIN_TABLE int do_builtin_cd(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_dot(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_exit(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_export(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_pwd(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_umask(int argc, std::string argv[]);


// the following functions execute Bourne again shell (bash) specific builtins
BUILTIN_TABLE int do_builtin_alias(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_echo(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_source(int argc, std::string argv[]):
BUILTIN_TABLE int do_builtin_unalias(int argc, std::string argv[]);


// job-control builtins
BUILTIN_TABLE int do_builtin_bg(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_fg(int argc, std::string argv[]);
BUILTIN_TABLE int do_builtin_jobs(int argc, std::string argv[]);


#endif