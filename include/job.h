/* File: job.h
 * Author: Joshua Jacobs-Rebhun
 * Date: December 4, 2021
 * 
 * This is the header file for the job class that represents a single job
 * entered from the command line.
 */



#ifndef JOB_H_
#define JOB_H_

#include <stdbool.h>
#include <vector>
#include <string>



/* 
 * maximum number of tokens (flags, args, etc.) 
 * that can be put into a single command. Chosen
 * so that it allows for a reasonably sized command
 * but does not use up too much system resources
 */
#define MAX_COMMAND_TOKENS 100




/*
 * Command class represents a single command in a given pipeline. It consists
 * of an array of strings that represent the command name, arguments, and flags.
 * In addition, it has a field that holds the length of the token array and
 * a list of files for each of input, output, and error redirection.
 */
class Command
{
private:

    // used for standard output redirection (i.e. ls > out.txt)
    bool _redirectOutput;
    bool _appendOutput;
    std::vector<std::string> _outputFiles;

    // used for standard input redirection (i.e. less < contacts.txt)
    bool _redirectInput;
    std::vector<std::string> _inputFiles;

    // used for standard error redirection (i.e. ls -l 2> /dev/null)
    bool _redirectError;
    std::vector<std::string> _errorFiles;

    // token array stores the command entered (without the redirection)
    int _numTokens;
    std::vector<std::string> _tokenArray;
    

public:


    /************************************
     * Public functions and constructors
     ************************************/



    // default constructor
    Command();
    
    
    /*
     * Constructor for Command copies the token array into the object.
     */
    Command(const std::vector<std::string>& tokenArray, \
        const std::vector<std::string>& outputFiles, const std::vector<std::string>& inputFiles, \
        const std::vector<std::string>& errorFiles, bool appendOutput);

    /*
     * Copy constructor for the command class
     */
    //Command(Command& command);
    


    // destructor
    ~Command();

    /*****************************************
     * Getters and setters for private fields
     *****************************************/

    /* no setters for boolean fields. These will be set
     * when the file list is set */
    bool isOutputRedirected();
    std::vector<std::string>& getOutputFiles();
    void addOutputFile(std::string outputFile);

    bool isOutputAppended();
    void setOutputAppended(bool appendOutput);

    bool isInputRedirected();
    std::vector<std::string>& getInputFiles();
    void addInputFile(std::string inputFile);

    bool isErrorRedirected();
    std::vector<std::string>& getErrorFiles();
    void addErrorFile(std::string errorFile);

    // numTokens has no setter. set when token array is set
    int getNumTokens();
    std::vector<std::string>& getTokenArray();
    void setTokenArray(std::vector<std::string>& tokenArray);
};

void operator<<(std::ostream& cout, Command& command);



/*
 * The class job is for storing the data needed to execute a given job.
 * The job may be composed of a single command or multiple commands 
 * connected by pipes and may be run in the foreground or in the background.
 * Additionally, there may be input redirection, output redirection, or
 * potentially both. The class has fields to store whether the job is running
 * in the background, whether there is input or output redirection, and the
 * number of commands in the given job. In addition, it has a field to store
 * an array of all of the Command objects.
 */
class Job
{
private:
    bool _background;
    int _numCommands;
    std::vector<Command> _commands;
    

public:
    Job();
    Job(bool background, int numCommands, const std::vector<Command>& commands);
    ~Job();

    bool isBackground();
    int getNumCommands();
    std::vector<Command>& getCommands();
    void setBackground(bool isBackground);
    void addCommand(const Command& command);
};



#endif