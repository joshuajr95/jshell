/*
 * File: test_parse.cc
 * Author: Joshua Jacobs-Rebhun
 * Date: December 26, 2021
 * 
 * This is the test file for the parsing of the input commands
 * to the command line. Takes a number of different commands and
 * parses them and checks whether the output is what was expected.
 */

#include <iostream>
#include <string>

#include <parse.h>
#include <job.h>


void testAllGetters(Command command)
{
    // print numTokens
    std::cout << "Num Tokens: " << command.getNumTokens() << std::endl;

    // get token array and print it out
    std::vector<std::string> tokenArray = command.getTokenArray();
    std::cout << "Token Array:";
    for(int i = 0; i < tokenArray.size(); i++)
    {
        std::cout << "\t" << tokenArray.at(i);
    }
    std::cout << std::endl;
    


    // print redirectOutput
    std::cout << "RedirectOutput: " << command.isOutputRedirected() << std::endl;

    // get output files and print them out
    std::vector<std::string> outputFiles = command.getOutputFiles();
    std::cout << "OutputFiles:";
    for(int i = 0; i < outputFiles.size(); i++)
    {
        std::cout << "\t" << outputFiles.at(i);
    }
    std::cout << std::endl;



    // print redirectInput
    std::cout << "RedirectInput: " << command.isInputRedirected() << std::endl;

    // get input files and print them out
    std::vector<std::string> inputFiles = command.getInputFiles();
    std::cout << "InputFiles:";
    for(int i = 0; i < inputFiles.size(); i++)
    {
        std::cout << "\t" << inputFiles.at(i);
    }
    std::cout << std::endl;

    // print redirectError
    std::cout << "RedirectError: " << command.isErrorRedirected() << std::endl;

    // get error files and print them out
    std::vector<std::string> errorFiles = command.getErrorFiles();
    std::cout << "ErrorFiles:";
    for(int i = 0; i < errorFiles.size(); i++)
    {
        std::cout << "\t" << errorFiles.at(i);
    }
    std::cout << std::endl;
}


void testAllGetters(Job job)
{
    std::cout << "Printing Job..." << std::endl;
    std::cout << "_________________\n" << "_________________\n";

    // print out whether job is background
    std::cout << "Background: " << job.isBackground() << std::endl;

    // print out the number of commands
    std::cout << "Num Commands: " << job.getNumCommands() << std::endl;

    std::vector<Command> commands = job.getCommands();
    for (int i = 0; i < commands.size(); i++)
    {
        Command command = commands.at(i);
        std::cout << "Command " << i << "\n___________\n\n";
        testAllGetters(command);
        std::cout << "\n\n";
    }
}


int main(int argc, char *argv[])
{
    std::string command1 = "cd ..";
    std::string command2 = "";
    std::string command3 = "ls -la | grep hello > out.txt";
    std::string command4 = "cat < file1.txt | grep thing | wc -l > outs.txt";
    std::string command5 = "cat < file1.txt > out.txt &";
    std::string command6 = "ls 1> out.txt 2> error.txt &";

    std::string badCommand1 = "ls >";


    Job job1 = getJob(command1);
    Job job2 = getJob(command2);
    Job job3 = getJob(command3);
    Job job4 = getJob(command4);
    Job job5 = getJob(command5);
    Job job6 = getJob(command6);

    
    testAllGetters(job1);
    testAllGetters(job2);
    testAllGetters(job3);
    testAllGetters(job4);
    testAllGetters(job5);
    testAllGetters(job6);

    Job badJob1 = getJob(badCommand1);

    testAllGetters(badJob1);
    

    return 0;
}