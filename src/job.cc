/* File: job.cc
 * Author: Joshua Jacobs-Rebhun
 * Date: December 4, 2021
 * 
 * This is the implementation file that defines all of the methods
 * for the Job and Command classes.
 */



#include <stdbool.h>
#include <iostream>
#include <vector>
#include <string>
#include <exception>


#include <job.h>




/* 
 * Default constructor sets redirection to false for all of
 * stdin, stdout, and stderr, and initializes redirection
 * files to empty vectors. It also sets token array to be
 * empty vector and sets numTokens to 0
 */
Command::Command()
{
    // stdin, stdout, and stderr redirection is disabled by default
    _redirectInput = false;
    _redirectOutput = false;
    _redirectError = false;
    _appendOutput = false;
    
    _numTokens = 0;
}


/*
 * Non-default constructor takes as input all of the necessary
 * information to create a command object. Since the user might
 * pass incorrect or malicious information (NULL pointer for example)
 * to the function, the constructor checks to make sure that the
 * pointers are not NULL before trying to dereference them.
 */
Command::Command(const std::vector<std::string>& tokenArray, const std::vector<std::string>& outputFiles, \
        const std::vector<std::string>& inputFiles, const std::vector<std::string>& errorFiles, bool appendOutput)
{
    _numTokens = tokenArray.size();
    _appendOutput = appendOutput;

    
    for(std::string token : tokenArray)
    {
        _tokenArray.push_back(token);
    }
    
    // setup output redirection
    for(std::string file : outputFiles)
    {
        _outputFiles.push_back(file);
    }

    
    if(outputFiles.size() > 0)
    {
        _redirectOutput = true;
    }
    else
    {
        _redirectOutput = false;
    }


    // setup input redirection
    for(std::string file : inputFiles)
    {
        _inputFiles.push_back(file);
    }

    if(inputFiles.size() > 0)
    {
        _redirectInput = true;
    }
    else
    {
        _redirectInput=false;
    }

    
    // setup error redirection
    for(std::string file : errorFiles)
    {
        _errorFiles.push_back(file);
    }

    if(errorFiles.size() > 0)
    {
        _redirectError = true;
    }
    else
    {
        _redirectError = false;
    }
}


/*
 * No need for NULL pointer checking on the redirection files and
 * token string for copy constructor because Command object cannot
 * be initialized with NULL pointer for any of the vectors.
 */

/*
Command::Command(Command& command)
{
    _appendOutput = command->isOutputAppended();
    _redirectOutput = command->isOutputRedirected();
    _redirectInput = command->isInputRedirected();
    _redirectError = command->isErrorRedirected();
    _numTokens = command->getNumTokens();

    std::vector<std::string> *inputFiles = command->getInputFiles();
    std::vector<std::string> *outputFiles = command->getOutputFiles();
    std::vector<std::string> *errorFiles = command->getErrorFiles();
    std::vector<std::string> *tokenArray = command->getTokenArray();

    this->inputFiles = new std::vector<std::string>();
    this->outputFiles = new std::vector<std::string>();
    this->errorFiles = new std::vector<std::string>();
    this->tokenArray = new std::vector<std::string>();


    for (int i = 0; i < inputFiles->size(); i++)
    {
        this->inputFiles->push_back(inputFiles->at(i));
    }

    for (int i = 0; i < outputFiles->size(); i++)
    {
        this->outputFiles->push_back(outputFiles->at(i));
    }
    
    for (int i = 0; i < errorFiles->size(); i++)
    {
        this->errorFiles->push_back(errorFiles->at(i));
    }
    
    for (int i = 0; i < tokenArray->size(); i++)
    {
        this->tokenArray->push_back(tokenArray->at(i));
    }
    
}
*/



// destructor
Command::~Command()
{
    _inputFiles.clear();
    _outputFiles.clear();
    _errorFiles.clear();
    _tokenArray.clear();
}

/*****************************************
 * Getters and setters for private fields
 *****************************************/

/* no setters for boolean fields. These will be set
    * when the file list is set */
bool Command::isOutputRedirected()
{
    return _redirectOutput;
}

bool Command::isOutputAppended()
{
    return _appendOutput;
}

void Command::setOutputAppended(bool appendOutput)
{
    _appendOutput = appendOutput;
}


std::vector<std::string>& Command::getOutputFiles()
{
    return _outputFiles;
}


void Command::addOutputFile(std::string outputFile)
{
    _redirectOutput = true;
    _outputFiles.push_back(outputFile);
}


bool Command::isInputRedirected()
{
    return _redirectInput;
}


std::vector<std::string>& Command::getInputFiles()
{
    return _inputFiles;
}


void Command::addInputFile(std::string inputFile)
{
    _redirectInput = true;
    _inputFiles.push_back(inputFile);
}


bool Command::isErrorRedirected()
{
    return _redirectError;
}


std::vector<std::string>& Command::getErrorFiles()
{
    return _errorFiles;
}


void Command::addErrorFile(std::string errorFile)
{
    _redirectError = true;
    _errorFiles.push_back(errorFile);
}


// numTokens has no setter. set when token array is set
int Command::getNumTokens()
{
    return _numTokens;
}


std::vector<std::string>& Command::getTokenArray()
{
    return _tokenArray;
}


void Command::setTokenArray(std::vector<std::string>& tokenArray)
{
    _numTokens = tokenArray.size();
    for (std::string token : tokenArray)
    {
        _tokenArray.push_back(token);
    }
    
}


// overload the insertion operator to print out the Command class
void operator<<(std::ostream& out, Command& command)
{
    // print output redirection files
    out << "Redirect Output: " << command.isOutputRedirected() << std::endl;
    out << "Output Files:";

    for (std::string file : command.getOutputFiles())
    {
        out << "\t" << file;
    }
    out << std::endl;
    

    // print input redirection files
    out << "Redirect Input: " << command.isInputRedirected() << std::endl;
    out << "Input Files:";

    for (std::string file : command.getInputFiles())
    {
        out << "\t" << file;
    }
    out << std::endl;


    // print error redirection files
    out << "Redirect Error: " << command.isErrorRedirected() << std::endl;
    out << "Error Files:";

    for (std::string file : command.getErrorFiles())
    {
        out << "\t" << file;
    }
    out << std::endl;


    // print out the token array
    out << "NumTokens: " << command.getNumTokens() << std::endl;
    out << "Token Array:";

    for (std::string token : command.getTokenArray())
    {
        out << "\t" << token;
    }
    out << std::endl;
    
}


/**************************
 * 
 **************************/


Job::Job()
{
    _background = false;
    _numCommands = 0;
}


Job::Job(bool background, int numCommands, const std::vector<Command>& commands)
{
    _background = background;
    _numCommands = numCommands;
    
    for(Command command : commands)
    {
        _commands.push_back(command);
    }
}


Job::~Job()
{
    _commands.clear();
}


bool Job::isBackground()
{
    return _background;
}


int Job::getNumCommands()
{
    return _numCommands;
}


std::vector<Command>& Job::getCommands()
{
    return _commands;
}


void Job::setBackground(bool isBackground)
{
    _background = isBackground;
}


void Job::addCommand(const Command& command)
{
    _commands.push_back(command);
    _numCommands++;
}

