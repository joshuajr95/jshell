/* File: parse.h
 * Author: Joshua Jacobs-Rebhun
 * Date: December 4, 2021
 * 
 * This header file defines a number of functions for parsing the
 * input to the command line and turning it into a job object.
 */

#ifndef PARSE_H_
#define PARSE_H_

#include <job.h>
#include <vector>


/*
 * Splits a string into a stream of tokens on the given delimiters.
 */
std::vector<std::string> tokenize(std::string inputString, std::string delimiters);


/*
 * This function takes the input command string and breaks it into
 * a sequence of tokens by splitting it at every space character.
 */
std::vector<std::string> tokenize(char *inputString, char *delimiters);

/*
 * This function takes the token sequence created by the previous function
 * and parses it to create the job object that stores the information for 
 * the command to be executed.
 */
Job parse_job(std::vector<std::string> tokens);

/*
 * Function that is called by parse_job to parse an individual command.
 */
Command parse_command(std::vector<std::string> tokens);


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
 */
Job getJob(std::string commandString);





#endif