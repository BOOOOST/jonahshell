#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
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
  size_t p = 0;
  while ((p = line.find(' ')) != std::string::npos){
    string sub = line.substr(0,p);
    if(sub == ""){
      line.erase(0,1);
    } else{
      args.push_back(sub);
      line.erase(0,p+1);
    }
  }
  args.push_back(line);
  return args;
}

int main()
{

  pid_t pid;        // child PID after fork
  string cmd_line;  // command line with arguments
  int wait_status;  // the status of the child after returning
  int argc=0;       // argument count
  
  while (true){
    cout << "MyShellPrompt$ ";
    getline (cin, cmd_line);
    if (cmd_line == string("exit")){
      cout << "Bye! End of Shell" << endl;
      break;
    }

    trim(cmd_line);
    vector<string> argstr = split(cmd_line);
    for(int i=0; i<argstr.size();i++){
      printf("argstr[%d] = %s\n",i,argstr[i].c_str());
    }

/*
    // prepare the input command for execution
    
    char * argv_str = new char [cmd_line.length()+1];   // allocate space for the NULL
    strcpy ( argv_str, cmd_line.c_str() );
      
    char * p = strtok(argv_str, " ");

    argc = 0;
    // here below we calculate the number of arguments comprizing the command line
    // value will be in argc
    while ( p !=0 ){
        argc++;
#ifdef DEBUG
        cout << p << '\n';
#endif
        p = strtok( NULL,  " " );
    }
#ifdef DEBUG
    cout << "argc = " << argc << '\n';
#endif

    // now argc has the number of arguments including the command and all options.
    // here we allocate argv and add 1 to the size for NULL as required by execvp()

    char *argv[argc + 1 ]; // account for last entry to be the NULL pointer for execvp

    // now rerun and build the argv vector contents and populate the entries

    strcpy ( argv_str, cmd_line.c_str() );  // reset the argv_str string
    p = strtok(argv_str, " ");
    argc = 0;  // reset argc
    while ( p !=0 ){
      argv[argc] = (char *) malloc(strlen(p));
      strcpy( argv[argc], p);
#ifdef DEBUG
      cout << argc << "-> " << argv[argc] << '\n';
      cout << p << '\n';
#endif
      argc++;
      p = strtok( NULL,  " " );
    }
      
    // now add the requisite NULL required by execvp
    argv[argc] = NULL;

    // now argv is populated with the command all options and arguments


    pid = fork();
    if (pid == 0){ // HERE WE ARE IN THE CHILD CODE
      
      int exec_retval = 0;
      exec_retval = execvp ( argv[0], argv );
#ifdef DEBUG      
      cout << exec_retval << '\n';
#endif
      if( exec_retval == -1 )
      {
        cout << "Command " << argv[0] << " not found! \n";
        break;
      }
    }else{         // HERE WE ARE IN THE PARENT CODE
      waitpid(pid, &wait_status, 0 ); // parent waits for child
    }
    */
  }
}