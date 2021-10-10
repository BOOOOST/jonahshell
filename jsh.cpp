#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include "executionplan.h"
//using std::string, std::cout, std::cin, std::getline, std::endl, std::strcpy, std::strtok;
using namespace std;
// required extern definition for exec family of commands

extern char **environ;

void trim (string& input){
  int i = 0;
  while(i < input.size() && input[i] == ' '){
    i++;
  }
  
  int j = input.size()-1;
  while(j>=0 && input[j] == ' '){
    j--;
  }
  input = input.substr(i,j-i+1);
}

vector<string> split (string line){
  vector<string> args;
  string sub;
  size_t p = 0;
  size_t q;
  size_t q2;
  while ((p = line.find(' ')) != std::string::npos){ //loop through trimmed line until no more spaces
    q = line.find('\''); //position of next single quote
    q2 = line.find('\"'); //position of next double quote
    if( ((q2 < p) && (line.find('\"',1) != std::string::npos)) ){//if double quote is before a space and there is another double quote to go with it
      sub = line.substr(1,line.find('\"',1)-1); //take the substring from the first character after the first quote and before the last quote
      args.push_back(sub); //push to args vector
      line.erase(0,line.find('\"',1)+1); // remove quoted string
    } else if( ((q < p) && (line.find('\'',1) != std::string::npos)) ){//same as above but for single quotes
      sub = line.substr(1,line.find('\'',1)-1);
      args.push_back(sub);
      line.erase(0,line.find('\'',1)+1);
    } else { //we have a space 
      sub = line.substr(0,p); // substring from beginning to the space position
      if(sub == ""){
        line.erase(0,1);//if next char from beginning is a space then remove it
      } else{
        args.push_back(sub);//otherwise add the substring to args
        line.erase(0,p+1); //remove added string and the space from line
      }
    }
  }
  //at this point the string has no more spaces
  //if remaining characters are quoted and remove the quotes if that is the case then add to args
  if(line.front() == '\'' && line.back() == '\'' || line.front() == '\"' && line.back() == '\"'){
    line = line.substr(1,line.size()-2);
  }
  args.push_back(line);
  return args;
}

int main()
{
 
  pid_t pid;  		// child PID after fork
  string cmd_line;	// command line with arguments
  int wait_status;	// the status of the child after returning
  int argc;		// argument count
  int in_fd = dup(0); // save the std in and out values
  int out_fd = dup(1);
  vector<pid_t> pidsvector; //vector of pids of zombie programs
  
  while (true){
    //for each zombie program we check if it is finished to reap it
    int b = 0;
    while(b < pidsvector.size()){
      if(waitpid (pidsvector[b],0,WNOHANG) == pidsvector[b]){
        pidsvector.erase(pidsvector.begin()+b);
      }else{
        b++;
      }
    }
    //reestablish stdin and stdout
    dup2(in_fd,0);
    dup2(out_fd,1);
    //get username for prompt
    char* username = getenv("USER");
    printf("%s@MyShellPrompt$ ",username);
    getline (cin, cmd_line);
    trim(cmd_line);
    if (cmd_line == string("exit")){
      cout << "Bye! End of Shell" << endl;
      break;
    }

    
    ExecutionPlan ep (split(cmd_line));
#ifdef DEBUG
    ep.printEP();
#endif

    int numcommands = ep.commands.size();
    int numargs;
    //for each command we do a fork
    for(int i = 0; i < numcommands; i++){
      int fd[2];//set up the pipe
      pipe(fd);
      numargs = ep.commands[i].size();
      //create array of char* with size numargs (+1 for null at end)
      char * cmd[numargs+1];
      for(int j = 0; j < numargs; j++){
        cmd[j] = (char *) malloc(strlen(ep.commands[i][j].c_str()));
        strcpy (cmd[j], ep.commands[i][j].c_str());
      }
      cmd[ep.commands[i].size()] = NULL;
      pid = fork();
      if (pid == 0){ // HERE WE ARE IN THE CHILD CODE
        if (i < ep.commands.size() - 1){
          dup2(fd[1], 1); //stdout -> pipe
        } else if(ep.stdoutRedirect != ""){
          //cannot use '>>' to append to file
          int outfile = open(ep.stdoutRedirect.c_str(), O_CREAT|O_WRONLY|O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
          dup2(outfile,1);
        }

        if(i == 0 && ep.stdinRedirect != ""){
          int infile = open(ep.stdinRedirect.c_str(), O_RDONLY, S_IREAD);
          dup2(infile, 0);
        }
        
        int exec_retval = 0;
        exec_retval = execvp ( cmd[0], cmd );
#ifdef DEBUG      
        cout << exec_retval << '\n';
#endif
        if( exec_retval == -1 ){
          cout << "Command " << cmd[0] << " not found! \n";
          return 0;
        }
      }else{         // HERE WE ARE IN THE PARENT CODE
        if(i == ep.commands.size()-1 && !ep.backGround){
          waitpid(pid, &wait_status, 0 ); // parent waits for child
        } else{
          pidsvector.push_back(pid);
        }

        //redirect the stdin for the next loop iteration
        dup2(fd[0],0); // stdin <- pipe
        close(fd[1]);
      }
    }
  }
  return 0;
}