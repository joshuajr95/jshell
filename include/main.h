#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <hashtable.h>
#include <job.h>
#include <builtin.h>

Table<std::string, std::string> alias_table;
Table<int, Job> job_table;
Table<std::string, Builtin> builtin_table;


#endif