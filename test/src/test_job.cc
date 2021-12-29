/*
 * File: test_job.cc
 * Author: Joshua Jacobs-Rebhun
 * Date Created: December 12, 2021
 * 
 * This file is a unit test for the job and command classes to make sure all of the fields
 * are accessible and the constructors are working correctly.
 */


#include <job.h>

#include <stdio.h>
#include <assert.h>
#include <vector>
#include <string>
#include <iostream>



void testAllGetters(Command *command)
{
    // print numTokens
    std::cout << "Num Tokens: " << command->getNumTokens() << std::endl;

    std::cout << "Got here\n";
    // get token array and print it out
    std::vector<std::string> *tokenArray = command->getTokenArray();
    std::cout << "Token Array:";
    for(int i = 0; i < tokenArray->size(); i++)
    {
        std::cout << "\t" << tokenArray->at(i);
    }
    std::cout << std::endl;



    // print redirectOutput
    std::cout << "RedirectOutput: " << command->isOutputRedirected() << std::endl;

    // get output files and print them out
    std::vector<std::string> *outputFiles = command->getOutputFiles();
    std::cout << "OutputFiles:";
    for(int i = 0; i < outputFiles->size(); i++)
    {
        std::cout << "\t" << outputFiles->at(i);
    }
    std::cout << std::endl;



    // print redirectInput
    std::cout << "RedirectInput: " << command->isInputRedirected() << std::endl;

    // get input files and print them out
    std::vector<std::string> *inputFiles = command->getInputFiles();
    std::cout << "InputFiles:";
    for(int i = 0; i < inputFiles->size(); i++)
    {
        std::cout << "\t" << inputFiles->at(i);
    }
    std::cout << std::endl;

    // print redirectError
    std::cout << "RedirectError: " << command->isErrorRedirected() << std::endl;

    // get error files and print them out
    std::vector<std::string> *errorFiles = command->getErrorFiles();
    std::cout << "ErrorFiles:";
    for(int i = 0; i < errorFiles->size(); i++)
    {
        std::cout << "\t" << errorFiles->at(i);
    }
    std::cout << std::endl;
}


void testAllGetters(Job *job)
{
    // print out whether job is background
    std::cout << "Background: " << job->isBackground() << std::endl;

    // print out the number of commands
    std::cout << "Num Commands: " << job->getNumCommands() << std::endl;

    std::vector<Command*> *commands = job->getCommands();
    for (int i = 0; i < commands->size(); i++)
    {
        Command *command = commands->at(i);
        std::cout << "Command " << i << "\n___________\n\n";
        testAllGetters(command);
        std::cout << "\n\n";
    }
    
}


int main(int argc, char *argv[])
{
    std::vector<std::string> tokenArray = {"ls", "-la"};
    std::vector<std::string> tokenArray2 = {"grep", "foo"};
    std::vector<std::string> tokenArray3 = {"wc", "-l"};

    Command *command1 = new Command();

    command1->setTokenArray(&tokenArray);
    command1->addOutputFile("output.txt");
    command1->addOutputFile("output2.txt");

    testAllGetters(command1);

    std::cout << std::endl << std::endl;

    command1->printCommand();

    Command *command2 = new Command();
    command2->setTokenArray(&tokenArray2);
    command2->addInputFile("grep_input.txt");
    
    
    testAllGetters(command2);


    Command *command3 = new Command();
    command3->setTokenArray(&tokenArray3);
    command3->addOutputFile("pipe_out.txt");
    command3->addErrorFile("errors.txt");

    Job *job1 = new Job();


    //testAllGetters(job1);

    job1->addCommand(command1);
    job1->addCommand(command2);
    job1->addCommand(command3);
    job1->setBackground(true);

    testAllGetters(job1);

    Command *command4 = job1->getCommands()->at(0);

    std::vector<Command*> someCommands;
    someCommands.push_back(command1);
    someCommands.push_back(command2);
    someCommands.push_back(command3);
    Job *job2 = new Job(true, 3, &someCommands);

    testAllGetters(job2);
    

    return 0;
}
