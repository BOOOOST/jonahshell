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

        vector<Command> commands;
        string stdinRedirect; //filename of the redirected stdin otherwise empty string
        string stdoutRedirect; //filename of the redirected stdout otherwise empty string
        bool backGround;

        ExecutionPlan(vector<string> inputs){ //expects inputs strings to be trimmed
            stdinRedirect = "";
            stdoutRedirect = "";
            for(int i = 0; i < inputs.size(); i++){ //look through all of the input vector
                if(inputs[i] == ">" && (i+1) < inputs.size()){ //if the i'th string is '>' or '<' and the next string exists
                    stdoutRedirect = inputs[i+1];              //then set the redirect to the name of the following string and remove both from the vector
                    inputs.erase(inputs.begin()+i);
                    inputs.erase(inputs.begin()+i);
                    i-=2;
                } else if(inputs[i] == "<" && (i+1) < inputs.size()){
                    stdinRedirect = inputs[i+1];
                    inputs.erase(inputs.begin()+i);
                    inputs.erase(inputs.begin()+i);
                    i-=2;
                }
            }
            if(inputs[inputs.size()-1] == "&"){
                backGround = true;
                inputs.erase(inputs.end());
            } else {
                backGround = false;
            }

            //need to create vector<string>'s separated by |'s to then push back onto commands
            if(inputs.size() != 0){
                Command cmd;
                for(int i = 0; i < inputs.size(); i++){
                    if(inputs[i] == "|"){
                        commands.push_back(cmd);
                        cmd.clear();
                    } else{
                        cmd.push_back(inputs[i]);
                    }
                }
                commands.push_back(cmd);
            }
        }

        ~ExecutionPlan(){}

        void printEP(){
            cout << "Is Background: " << backGround << endl;
            cout << "stdin redirect: " << stdinRedirect << endl;
            cout << "stdout redirect: " << stdoutRedirect << endl;
            if(commands.size() > 0){
                for(int i = 0; i < commands.size(); i++){
                    printf("commands[%d] = ",i);
                    for(int j = 0; j < commands[i].size()-1; j++){
                        cout << commands[i][j] << ", ";
                    }
                    cout << commands[i][commands[i].size()-1] << endl;
                }
            } else {
                cout << "Commands is empty.\n"; 
            }
        }
};
#endif