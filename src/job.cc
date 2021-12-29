/* File: job.cc
 * Author: Joshua Jacobs-Rebhun
 * Date: December 4, 2021
 * 
 * This is the implementation file that defines all of the methods
 * for the Job and Command classes.
 */



#include <stdbool.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
#include <exception>


/* 
 * maximum number of tokens (flags, args, etc.) 
 * that can be put into a single command. Chosen
 * so that it allows for a reasonably sized command
 * but does not use up too much system resources
 */
#define MAX_COMMAND_TOKENS 100



/*
 * Command class uses array of char* instead of vector because this is the
 * form that exec uses for argument vector and converting from a std::vector
 * object to an array would waste time.
 */
class Command
{
private:

    // used for standard output redirection (i.e. ls > out.txt)
    bool redirectOutput;
    bool appendOutput;
    std::vector<std::string> *outputFiles;

    // used for standard input redirection (i.e. less < contacts.txt)
    bool redirectInput;
    std::vector<std::string> *inputFiles;

    // used for standard error redirection (i.e. ls -l 2> /dev/null)
    bool redirectError;
    std::vector<std::string> *errorFiles;

    // token array stores the command entered (without the redirection)
    int numTokens;
    std::vector<std::string> *tokenArray;
    

public:


    /************************************
     * Public functions and constructors
     ************************************/



    // default constructor
    Command();
    
    
    /*
     * Constructor for Command copies the token array into the object.
     */
    Command(std::vector<std::string> *tokenArray, \
        std::vector<std::string> *outputFiles, std::vector<std::string> *inputFiles, \
        std::vector<std::string> *errorFiles, bool appendOutput);

    /*
     * Copy constructor for the command class
     */
    Command(Command *command);
    


    // destructor
    ~Command();

    /*****************************************
     * Getters and setters for private fields
     *****************************************/

    /* no setters for boolean fields. These will be set
     * when the file list is set */
    bool isOutputRedirected();
    std::vector<std::string> *getOutputFiles();
    void addOutputFile(std::string outputFile);

    bool isOutputAppended();
    void setOutputAppended(bool appendOutput);

    bool isInputRedirected();
    std::vector<std::string> *getInputFiles();
    void addInputFile(std::string inputFile);

    bool isErrorRedirected();
    std::vector<std::string> *getErrorFiles();
    void addErrorFile(std::string errorFile);

    // numTokens has no setter. set when token array is set
    int getNumTokens();
    std::vector<std::string> *getTokenArray();
    void setTokenArray(std::vector<std::string> *tokenArray);

    // print out the command to the user (mostly used for testing)
    void printCommand();

    
};


/* 
 * Default constructor sets redirection to false for all of
 * stdin, stdout, and stderr, and initializes redirection
 * files to empty vectors. It also sets token array to be
 * empty vector and sets numTokens to 0
 */
Command::Command()
{
    // stdin, stdout, and stderr redirection is disabled by default
    this->redirectInput = false;
    this->redirectOutput = false;
    this->redirectError = false;
    this->appendOutput = false;

    // redirection files are set to empty list by default
    this->inputFiles = new std::vector<std::string>();
    this->outputFiles = new std::vector<std::string>();
    this->errorFiles = new std::vector<std::string>();

    // set up empty command string to prevent accessing null data
    this->numTokens = 0;
    this->tokenArray = new std::vector<std::string>();
}


/*
 * Non-default constructor takes as input all of the necessary
 * information to create a command object. Since the user might
 * pass incorrect or malicious information (NULL pointer for example)
 * to the function, the constructor checks to make sure that the
 * pointers are not NULL before trying to dereference them.
 */
Command::Command(std::vector<std::string> *tokenArray, std::vector<std::string> *outputFiles, \
        std::vector<std::string> *inputFiles, std::vector<std::string> *errorFiles, bool appendOutput)
{
    this->numTokens = tokenArray->size();
    this->appendOutput = appendOutput;

    if(tokenArray == NULL)
    {
        this->tokenArray = new std::vector<std::string>();
    }
    else
    {
        this->tokenArray = new std::vector<std::string>(*tokenArray);
    }


    // setup output redirection
    if(outputFiles == NULL)
    {
        this->outputFiles = new std::vector<std::string>();
    }
    else
    {
        this->outputFiles = new std::vector<std::string>(*outputFiles);
    }
    
    if(outputFiles->size() > 0)
    {
        this->redirectOutput = true;
    }
    else
    {
        this->redirectOutput = false;
    }


    // setup input redirection
    if(inputFiles == NULL)
    {
        this->inputFiles = new std::vector<std::string>();
    }
    else
    {
        this->inputFiles = new std::vector<std::string>(*inputFiles);
    }

    if(inputFiles->size() > 0)
    {
        this->redirectInput = true;
    }
    else
    {
        this->redirectInput=false;
    }

    
    // setup error redirection
    if(errorFiles == NULL)
    {
        this->errorFiles = new std::vector<std::string>();
    }
    else
    {
        this->errorFiles = new std::vector<std::string>(*errorFiles);
    }

    if(errorFiles->size() > 0)
    {
        this->redirectError = true;
    }
    else
    {
        this->redirectError = false;
    }
}


/*
 * No need for NULL pointer checking on the redirection files and
 * token string for copy constructor because Command object cannot
 * be initialized with NULL pointer for any of the vectors.
 */
Command::Command(Command *command)
{
    this->appendOutput = command->isOutputAppended();
    this->redirectOutput = command->isOutputRedirected();
    this->redirectInput = command->isInputRedirected();
    this->redirectError = command->isErrorRedirected();
    this->numTokens = command->getNumTokens();

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



// destructor
Command::~Command()
{
    inputFiles->clear();
    delete inputFiles;

    outputFiles->clear();
    delete outputFiles;

    errorFiles->clear();
    delete errorFiles;

    tokenArray->clear();
    delete tokenArray;
}

/*****************************************
 * Getters and setters for private fields
 *****************************************/

/* no setters for boolean fields. These will be set
    * when the file list is set */
bool Command::isOutputRedirected()
{
    return this->redirectOutput;
}

bool Command::isOutputAppended()
{
    return this->appendOutput;
}

void Command::setOutputAppended(bool appendOutput)
{
    this->appendOutput = appendOutput;
}


std::vector<std::string> *Command::getOutputFiles()
{
    return this->outputFiles;
}


void Command::addOutputFile(std::string outputFile)
{
    this->redirectOutput = true;
    this->outputFiles->push_back(outputFile);
}


bool Command::isInputRedirected()
{
    return redirectInput;
}


std::vector<std::string> *Command::getInputFiles()
{
    return this->inputFiles;
}


void Command::addInputFile(std::string inputFile)
{
    this->redirectInput = true;
    this->inputFiles->push_back(inputFile);
}


bool Command::isErrorRedirected()
{
    return redirectError;
}


std::vector<std::string> *Command::getErrorFiles()
{
    return this->errorFiles;
}


void Command::addErrorFile(std::string errorFile)
{
    this->redirectError = true;
    this->errorFiles->push_back(errorFile);
}


// numTokens has no setter. set when token array is set
int Command::getNumTokens()
{
    return numTokens;
}


std::vector<std::string> *Command::getTokenArray()
{
    return this->tokenArray;
}


void Command::setTokenArray(std::vector<std::string> *tokenArray)
{
    this->numTokens = tokenArray->size();
    for (int i = 0; i < this->numTokens; i++)
    {
        std::string token = tokenArray->at(i);
        this->tokenArray->push_back(token);
    }
    
}


/*************************
 * Print out the command *
 *************************/
void Command::printCommand()
{
    // print output redirection files
    std::cout << "Redirect Output: " << this->redirectOutput << std::endl;
    std::cout << "Output Files:";

    for (int i = 0; i < this->outputFiles->size(); i++)
    {
        std::string outfile = outputFiles->at(i);
        std::cout << "\t" << outfile;
    }
    std::cout << std::endl;
    

    // print input redirection files
    std::cout << "Redirect Input: " << this->redirectInput << std::endl;
    std::cout << "Input Files:";

    for (int i = 0; i < this->inputFiles->size(); i++)
    {
        std::string infile = inputFiles->at(i);
        std::cout << "\t" << infile;
    }
    std::cout << std::endl;


    // print error redirection files
    std::cout << "Redirect Input: " << this->redirectInput << std::endl;
    std::cout << "Input Files:";

    for (int i = 0; i < this->inputFiles->size(); i++)
    {
        std::string infile = inputFiles->at(i);
        std::cout << "\t" << infile;
    }
    std::cout << std::endl;


    // print out the token array
    std::cout << "NumTokens: " << this->numTokens << std::endl;
    std::cout << "Token Array:";

    for (int i = 0; i < this->tokenArray->size(); i++)
    {
        std::string token = this->tokenArray->at(i);
        std::cout << "\t" << token;
    }
    std::cout << std::endl;
    
}



/*
 * Job class represents an entire Unix pipeline with a number of individual commands
 * connected by pipes. It can be run in the foreground or in the background.
 */
class Job
{
private:

    // used for whether command should be run in the background or not
    bool background;

    int numCommands;
    std::vector<Command*> *commands;
    

public:

    /*
     * Default constructor
     */
    Job();

    /*
     * Constructor for Job object must copy all of the command objects
     * into the Job object and set the appropriate variables.
     */
    Job(bool background, int numCommands, std::vector<Command*> *commands);
    ~Job();
    bool isBackground();
    int getNumCommands();
    std::vector<Command*> *getCommands();
    void setBackground(bool isBackground);
    void addCommand(Command *command);
    
};


Job::Job()
{
    this->background = false;
    this->numCommands = 0;
    this->commands = new std::vector<Command*>();
}


Job::Job(bool background, int numCommands, std::vector<Command*> *commands)
{
    this->background = background;
    this->numCommands = numCommands;
    this->commands = new std::vector<Command*>();

    for(int i = 0; i < numCommands; i++)
    {
        this->commands->push_back(commands->at(i));
    }
}


Job::~Job()
{
    commands->clear();
    delete commands;
}


bool Job::isBackground()
{
    return this->background;
}


int Job::getNumCommands()
{
    return this->numCommands;
}


std::vector<Command*> *Job::getCommands()
{
    return this->commands;
}


void Job::setBackground(bool isBackground)
{
    this->background = isBackground;
}


void Job::addCommand(Command *command)
{
    this->commands->push_back(new Command(command));
    this->numCommands++;
}

