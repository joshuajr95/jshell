#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <hashtable.h>
#include <job.h>
#include <builtin.h>
#include <signal_handlers.h>

Table<std::string, std::string> alias_table;
Table<int, Job> job_table;
Table<std::string, builtin_t> builtin_table;
Table<int, sighandler_t> sighandler_table;

void initialize_builtin_table();
void initialize_job_table();
void initialize_alias_table();
void initialize_sighandler_table();

#endif