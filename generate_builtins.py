#! /usr/bin/python3


import os

# input file path
infile = "builtin.h"
include_dir = "include"
inpath = include_dir + "/" + infile

# output file path
outfile = "builtin_list.h"
outpath = include_dir + "/" + outfile


# check if generated file exists, and if so remove it before remaking it
if outfile in os.listdir(include_dir):
    os.remove(outpath)


input_file_handle = open(inpath, "r")
output_file_handle = open(outpath, "w")

output_file_handle.write("#ifndef BUILTIN_LIST_H\n")
output_file_handle.write("#define BUILTIN_LIST_H\n\n\n")
output_file_handle.write("#include <builtin.h>\n\n")

builtin_list = "const builtin_t builtin_list[] = {"
builtin_commands = "const char *builtin_commands_list[] = {"

first_occurrence = True
num_builtins = 0

for line in input_file_handle:
    if line.startswith("BUILTIN_TABLE"):
        line_list = line.split()
        builtin_function_name = line_list[2].split("(")[0]
        builtin_command_name = builtin_function_name.split("_")[2]

        if first_occurrence:
            builtin_list += builtin_function_name
            builtin_commands = builtin_commands + "\"" + builtin_command_name + "\""
            first_occurrence = False
        else:
            builtin_list = builtin_list + ", " + builtin_function_name
            builtin_commands = builtin_commands + ", " + "\"" + builtin_command_name + "\""
        
        num_builtins += 1

builtin_list += "};\n\n"
builtin_commands += "};\n\n"
output_file_handle.write(builtin_list)
output_file_handle.write(builtin_commands)
output_file_handle.write("int num_builtins = ")
output_file_handle.write(str(num_builtins))
output_file_handle.write(";\n\n")
output_file_handle.write("#endif")

output_file_handle.close()
input_file_handle.close()
