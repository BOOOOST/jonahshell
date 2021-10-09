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