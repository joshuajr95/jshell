#! /usr/bin/python3

import os


input_file = "signal_handlers.h"
input_dir = "include"
input_file_path = input_dir + "/" + input_file


output_file = "sighandler_list.h"
output_dir = "include"
output_file_path = output_dir + "/" + output_file


if output_file in os.listdir(output_dir):
    os.remove(output_file_path)


input_file_handle = open(input_file_path, "r")
output_file_handle = open(output_file_path, "w")

output_file_handle.write("#ifndef SIGHANDLER_LIST_H\n")
output_file_handle.write("#define SIGHANDLER_LIST_H\n\n")
output_file_handle.write("#include <signal_handlers.h>\n\n")

signal_handler_list = "sighandler_t signal_handler_list[] = {"
signum_list = "int signum_list[] = {"

first_occurrence = True
num_sighandlers = 0

for line in input_file_handle:
    if line.startswith("SIGNAL_TABLE"):
        splitline = line.split()
        sighandle = splitline[2].split("(")[0]
        
        if first_occurrence:
            first_occurrence = False
            signal_handler_list += sighandle
            signum_list += sighandle.split("_")[1].upper()
            num_sighandlers += 1
        else:
            signal_handler_list = signal_handler_list + ", " + sighandle
            signum_list = signum_list + ", " + sighandle.split("_")[1].upper()
            num_sighandlers += 1

output_file_handle.write(signal_handler_list)
output_file_handle.write("};\n")
output_file_handle.write(signum_list)
output_file_handle.write("};\n")

output_file_handle.write("int num_sighandlers = ")
output_file_handle.write(str(num_sighandlers))
output_file_handle.write(";\n\n")

output_file_handle.write("#endif")