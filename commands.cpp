//		commands.cpp
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
using namespace std;
class job{
        public:
        unsigned int pid;
        unsigned int job_id;
        unsigned int elp_sec;
        char* command;
        char* status;
};
//**************************************************************************************

static char OLDPWD = NULL;

int ExeCmd(void* jobs, char* lineSize, char* cmdString)
{
	char* cmd; 
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
    	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
   	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 
 
	}
/*************************************************/
// Built in Commands PLEASE NOTE NOT ALL REQUIRED
// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
// MORE IF STATEMENTS AS REQUIRED
/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{
		(num_arg != 0)? (illegal_cmd = true) : (illegal_cmd = false);
        if(num_arg > 1){
        	illegal_cmd = true;
           cout << "smash error: cd: too many arguments" << endl;
        }
        else{
            illegal_cmd = false;
        }
            if(!illegal_cmd)
              {
                char* curr_dir[MAX_LINE_SIZE];
                if(args[1] == "-"){
                    if(!OLDPWD){
                        cout << "smash error: cd: OLDPWD not set" <<endl;
                    }
                    else{
                        *curr_dir = &OLDPWD;
                        getcwd(OLDPWD, MAX_LINE_SIZE);
                        chdir(curr_dir);
                        cout << curr_dir << endl;
                    }
                }
                else{
                    getcwd(curr_dir, MAX_LINE_SIZE);
                    strcat(curr_dir,(const char*) arg[1]);
                    chdir(curr_dir);
                    cout << curr_dir << endl;
                }
               
              }
	} 
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
       /* (num_arg != 0)? (illegal_cmd = true) : (illegal_cmd = false);
         if(!illegal_cmd)
          {
            char curr_dir[MAX_LINE_SIZE];
            getcwd(curr_dir, MAX_LINE_SIZE);
            cout << curr_dir << endl;
          }*/
	}
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
		  (num_arg != 2)? (illegal_cmd = true) : (illegal_cmd = false);
		   if(!illegal_cmd)
		   {
			 FILE *file1 = fopen(*args[1],"r");
			 FILE *file2 = fopen(*args[2],"r");
			 if (file1 == NULL || file2 == NULL){
			     printf("Error : Files not open");
			     exit(0);
			 }
			 char ch1 = getc(file1);
			 char ch2 = getc(file2);
			 while (ch1 != EOF && ch2 != EOF){
				 if (ch1 != ch2){
					 cout << "1"<< endl; 
					 break;
				 }
				 ch1 = getc(file1);
				 ch2 = getc(file2);
			 }
			 if (ch1 == EOF && ch2 == EOF){
				 cout << "0"<< endl;
			 }
			 else cout << "1"<< endl;
			        
		   }
	}
	/*************************************************/
	
	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{
 		
	}
	/*************************************************/
	
	else if (!strcmp(cmd, "jobs")) 
	{
 		
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid"))
	{
        (num_arg != 0)? (illegal_cmd = true) : (illegal_cmd = false);
        if (!illegal_cmd)
        {
            cout << "smash pid is " << getpid() << endl;
        }
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{
		
	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
  		
	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{
   		
	} 
	/*************************************************/
	else // external command
	{
 		ExeExternal(args, cmdString);
	 	return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
    	switch(pID = fork()) 
	{
    		case -1: 
					// Add your code here (error)
					
					/* 
					your code
					*/
        	case 0 :
                	// Child Process
               		setpgrp();
					
			        // Add your code here (execute an external command)
					
					/* 
					your code
					*/
			
        default:;
                	// Add your code here
					
					/* 
					your code
					*/
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
		// Add your code here (execute a complicated command)
					
		/* 
		your code
		*/
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, void* jobs)
{

	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)
					
		/* 
		your code
		*/
		
	}
	return -1;
}

