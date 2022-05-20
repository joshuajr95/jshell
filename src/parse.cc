/* File: parse.cc
 * Author: Joshua Jacobs-Rebhun
 * Date: Wednesday December 1, 2021
 * 
 * This file implements the language processing for the shell program
 * including the lexing, parsing, and generation of job object for
 * input into the jshell main program.
 */


//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>

#include <job.h>
#include <parse.h>



/*
 * This function is equivalent to strtok or strtok_r, except for two
 * things. First, it returns a vector of tokens to make it easier to
 * program (i.e. by just appending each time a token is detected).
 * And second, it doesn't fucking suck.
 */
std::vector<std::string> tokenize(std::string inputString, std::string delimiters)
{
    std::vector<std::string> tokens;

    int start = 0;
    int stop = 0;

    while(stop < inputString.length())
    {
        if(stop == inputString.length() - 1)
        {
            std::string token = inputString.substr(start, stop-start+1);
            tokens.push_back(token);
            break;
        }

        for (int i = 0; i < delimiters.length(); i++)
        {
            if(inputString[stop] == delimiters[i])
            {
                std::string token = inputString.substr(start, stop-start);
                tokens.push_back(token);
                start = stop+1;
                break;
            }
        }

        stop++;
    }
    

    return tokens;
}




/*
 * This function takes the input command string and breaks it into
 * a sequence of tokens by splitting it at every space character.
 */
std::vector<std::string> tokenize(char *inputString, char *delimiters)
{

    // stores split string
    std::vector<std::string> tokens;

    char *token;
    char *rest = inputString;

    while((token = strtok_r(rest, delimiters, &rest)))
    {
        std::string *stoken = new std::string(token);
        tokens.push_back(*stoken);
        delete stoken;
    }

    return tokens;
}



/*
 * Parses an individual command as opposed to an entire pipeline, as above.
 * This function is recursively called by the parse_job function. This is
 * a kind of top-down recursive parsing.
 */
Command parse_command(std::vector<std::string> tokens)
{
    // index of the last part of the command that is not redirection
    int lastFlagIndex = -1;

    // create new command to store result of parsing
    Command command;


    int i = 0;
    while(i < tokens.size())
    {
        std::string currentToken = tokens.at(i);

        

        // simple output redirection denoted by >, or by 1>
        if(currentToken.compare(">") == 0 || currentToken.compare("1>") == 0)
        {
            
            if(i+1 < tokens.size())
            {
                command.addOutputFile(tokens.at(i+1));
            }
            else
            {
                throw std::runtime_error("Bad command: incorrect syntax.");
            }
            
            
            

            if(lastFlagIndex < 0)
            {
                lastFlagIndex = i - 1;
            }

            i += 2;
        }

        // input redirection denoted by <
        else if(currentToken.compare("<") == 0)
        {
            
            if(i+1 < tokens.size())
            {
                command.addInputFile(tokens.at(i+1));
            }
            else
            {
                throw std::runtime_error("Bad command: incorrect syntax.");
            }


            if(lastFlagIndex < 0)
            {
                lastFlagIndex = i - 1;
            }

            i += 2;
        }

        // error redirection denoted by 2>
        else if(currentToken.compare("2>") == 0)
        {
            if(i+1 < tokens.size())
            {
                command.addErrorFile(tokens.at(i+1));
            }
            else
            {
                throw std::runtime_error("Bad command: incorrect syntax.");
            }
            
            

            if(lastFlagIndex < 0)
            {
                lastFlagIndex = i - 1;
            }

            i += 2;
        }

        // simultaneous output and error redirection denoted by &>
        else if(currentToken.compare("&>") == 0)
        {
            if(i+1 < tokens.size())
            {
                command.addErrorFile(tokens.at(i+1));
                command.addOutputFile(tokens.at(i+1));
            }
            else
            {
                throw std::runtime_error("Bad command: incorrect syntax.");
            }
            
            
            

            if(lastFlagIndex < 0)
            {
                lastFlagIndex = i - 1;
            }

            i += 2;
        }

        else
        {
            i++;
        }
    }


    /*
     * if there is no redirection, then set the last flag
     * index to be the index of the last token in the array
     */
    if(lastFlagIndex < 0)
        lastFlagIndex = tokens.size()-1;
    
    // fill commandTokens array with tokens for command itself
    std::vector<std::string> commandTokens;
    for (int i = 0; i <= lastFlagIndex; i++)
    {
        commandTokens.push_back(tokens.at(i));
    }

    command.setTokenArray(commandTokens);

    return command;
    
}




/*
 * This function takes the token sequence created by the previous function
 * and parses it to create the Job object that stores the information for 
 * the command to be executed. The function recursively parses the pipeline
 * entered by first deciding whether to run the job in the background, then
 * splitting the job into individual commands, and then parsing the individual
 * commands by calling parse_command.
 */
Job parse_job(std::vector<std::string> tokens)
{

    Job job;

    if(tokens.size() == 0)
    {
        return job;
    }
    

    /* 
     * if the token string ends with '&', then command should be
     * run in the background. Otherwise foreground.
     */
    if(tokens.back().compare("&") == 0)
    {
        job.setBackground(true);
        tokens.pop_back();
    }
    else
    {
        job.setBackground(false);
    }



    // start and stop indices are used to parse the token sequence
    int start = 0;
    int stop = 0;

    while(stop < tokens.size())
    {
        // if iterating variable has reached end, add new command
        if(stop == tokens.size() - 1)
        {
            /*
             * copy portion of token vector to new token vector
             * for recursive parsing
             */
            auto begin = tokens.begin() + start;
            auto end = tokens.end();
            std::vector<std::string> commandTokens(stop-start+1);
            std::copy(begin, end, commandTokens.begin());


            /*
             * generate new command object by recursively parsing
             * using the parse_command routine
             */
            Command command = parse_command(commandTokens);
            job.addCommand(command);
            
            
            stop++;
        }

        // if there is a pipe, add new command
        else if(tokens.at(stop).compare("|") == 0)
        {
            /*
             * Extract partial token vector from whole for
             * recursive parsing
             */
            auto begin = tokens.begin() + start;
            auto end = tokens.begin() + stop;
            std::vector<std::string> commandTokens(stop-start);
            std::copy(begin, end, commandTokens.begin());


            // recursively parse individual command and append it to command vector
            Command command = parse_command(commandTokens);
            job.addCommand(command);

            // increment iterators
            stop++;
            start = stop;

        }

        // otherwise, simply increase stop
        else
        {
            stop++;
        }

        
    }


    return job;
}



/*
 * This function is the wrapper function for the lexical analysis
 * and parsing of the command. This is made so that the calling code
 * does not have to know about the inner workings of the parsing
 * process and can instead simply call the getCommand function on
 * the command string entered by the user into the command line
 * and the function will simply return a pointer to a dynamically
 * allocated Job object. This allows the actual implementation code
 * to be changed in the future to allow more complex or efficient
 * algorithms.
 * 
 * WARNING: Throws an exception if the input is not in the correct format.
 */
Job getJob(std::string commandString)
{
    std::vector<std::string> tokens = tokenize(commandString, " ");
    return parse_job(tokens);
}


/*
 * Overloaded version of the previous function for interoperability
 * with C code.
 */
Job getJob(char *commandString)
{
    return parse_job(tokenize(commandString, " "));
}