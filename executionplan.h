#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#ifndef HEADER_H
#define HEADER_H
using namespace std;
using Command = vector<string>;

class ExecutionPlan {
    public:

        ExecutionPlan(){
            backGround = false;
        }
        
    private:

        vector<Command> commands;
        string stdinRedirect;
        string stdoutRedirect;
        bool backGround;
};
#endif

// psudo code 
// assume the command read in from input is 'ls -l'
// then vector<Command> commands woud contain:
//           "ls" and "-l"
// to convert to a c string (ie char *)
//
// loop over commands vector to count length of the 
// command and option strings
// int cmd_str_len = 0;
// foreach (string cmdstr : command)
// {   // note this does not account for spaces in between the options
//     cmd_str_len += cmdstr.size();
// }
//
// // convert to char *
// char * cmd = new char[cmd_str_len];
// char * cmd_pointer_copy = cmd;
// strcpy ( argv_str, cmd_line.c_str() );
// foreach (string cmdstr : command)
// {   // note: again this does not account for spaces in between the options
//     strcpy (cmd_pointer_copy, cmdstr.c_str() );
//     cmd_pointer_copy += cmdstr.size();
// }
//
//now cmd should be a pointer to a c string of the command to execute
