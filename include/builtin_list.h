#ifndef BUILTIN_LIST_H
#define BUILTIN_LIST_H


#include <builtin.h>

const builtin_t builtin_list[] = {do_builtin_cd, do_builtin_dot, do_builtin_exit, do_builtin_export, do_builtin_pwd, do_builtin_umask, do_builtin_unset, do_builtin_alias, do_builtin_echo, do_builtin_kill, do_builtin_source, do_builtin_unalias, do_builtin_bg, do_builtin_fg, do_builtin_jobs};

const char *builtin_commands_list[] = {"cd", "dot", "exit", "export", "pwd", "umask", "unset", "alias", "echo", "kill", "source", "unalias", "bg", "fg", "jobs"};

int num_builtins = 15;

#endif