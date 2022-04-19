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
//******************************((funcs))***********************************************
void Job::Jprint(){
    if (!strcmp(this->status, "stopped")){
        cout <<"["<<job_id<<"]"<<command<<":"<<pid<< " " << difftime(time(NULL), elp_sec)<<"secs"<<" (Stopped)"<< endl;
        }
    else{
        cout <<"["<<job_id<<"]"<<command<<":"<<pid<< " " << difftime(time(NULL), elp_sec)<<"secs"<< endl;
    }
};
Job::Job(unsigned int pid,unsigned int job_id,unsigned int elp_sec,char* command,char* status){
            this->pid = pid;
            this->job_id = job_id;
            this->elp_sec = elp_sec;
            strcpy(this->command,command);
            strcpy(this->status,status);
}

//**************************************************************************************
char OLDPWD[MAX_LINE_SIZE];
static unsigned int jobs_counter = 1;

int ExeCmd( char* lineSize, char* cmdString)
{
	char* cmd;
	char* args[MAX_ARG];
	char pwd[MAX_LINE_SIZE];
	char delimiters[4] = " \t\n";
	int i = 0, num_arg = 0;
    pid_t w;
    int status;
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
                if (num_arg != 1)
                {
                    illegal_cmd = true;
                    perror( "smash error: cd: too many arguments");
                }
                if (!strcmp(args[1], "-"))
                {
                    illegal_cmd = false;
                    if (OLDPWD[0] != 0)
                    {
                        char curr_dir[MAX_LINE_SIZE];
                        if (getcwd(curr_dir, MAX_LINE_SIZE) == NULL)
                        {
                            perror("smash error: ");
                            return 1;
                        }
                        if (chdir(OLDPWD) == -1)
                        {
                            perror("smash error: cd: OLDPWD not set");
                            return 1;
                        }
                        strcpy(OLDPWD, curr_dir);
                    }

                }
                else
                {
                    char curr_dir[MAX_LINE_SIZE];
                    if (getcwd(curr_dir, MAX_LINE_SIZE) == NULL)
                    {
                        perror("smash error: ");
                        return 1;
                    }
                    if (chdir(args[1]) == -1)
                    {
                        cout << "smash error: cd:" << args[1] <<" not set" <<endl;
                        return 1;
                    }
                    strcpy(OLDPWD, curr_dir);
                }
    }
	
	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
        (num_arg != 0)? (illegal_cmd = true) : (illegal_cmd = false);
         if(!illegal_cmd)
          {
            char curr_dir[MAX_LINE_SIZE];
            getcwd(curr_dir, MAX_LINE_SIZE);
            cout << curr_dir << endl;
          }
	}
	/*************************************************/
	else if (!strcmp(cmd, "diff"))
	{
		  (num_arg != 2)? (illegal_cmd = true) : (illegal_cmd = false);
		   if(!illegal_cmd)
		   {
			 FILE *file1 = fopen(args[1],"r");
			 FILE *file2 = fopen(args[2],"r");
			 if (file1 == NULL || file2 == NULL){
			     printf("Error : Files not open");
			     exit(0);
			 }
			 char ch1 = getc(file1);
			 char ch2 = getc(file2);
			 while (ch1 != EOF && ch2 != EOF){
				 if (ch1 != ch2){
					 cout << "1" << endl;
					 break;
				 }
				 ch1 = getc(file1);
				 ch2 = getc(file2);
			 }
			 if (ch1 == EOF && ch2 == EOF){
				 cout << "0"<< endl;
			 }
			 //else cout << "1"<< endl;
			        
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
        vector<Job>::iterator it =jobs.begin();
        while(it != jobs.end()) {
            it->Jprint();
            it++;
        }
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
        vector<Job>::iterator it;
        if(num_arg > 1){ //maybe need to add more conditions
            cout << "smash error: fg: invalid arguments" << endl;
        }
        else{
            if(num_arg == 1){
                int i = 0;
                for(vector<Job>::iterator it = jobs.begin(); it != jobs.end(); it++)
                {
                    if(it->pid == *args[1]){
                        do {
                            w = waitpid((pid_t)it->pid, &status, WUNTRACED | WCONTINUED);
                            if (w == -1) { perror("waitpid"); exit(EXIT_FAILURE); }


                            if (WIFEXITED(status)) {
                                printf("exited, status=%d\n", WEXITSTATUS(status));
                            } else if (WIFSIGNALED(status)) {
                                printf("killed by signal %d\n", WTERMSIG(status));
                            } else if (WIFSTOPPED(status)) {
                                printf("stopped by signal %d\n", WSTOPSIG(status));
                            } else if (WIFCONTINUED(status)) {
                                printf("continued\n");
                            }
                        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                        exit(EXIT_SUCCESS);
                        jobs.erase(it);
                        break;
                    }
                }
            }

            else if(num_arg == 0){
                    if(!jobs.size()) {
                        cout << "smash error: fg: jobs list is empty" << endl;
                    }
                    else{
                        it = jobs.end();
                        do {
                            w = waitpid((pid_t)it->pid, &status, WUNTRACED | WCONTINUED);
                            if (w == -1) { perror("waitpid"); exit(EXIT_FAILURE); }

                            if (WIFEXITED(status)) {
                                printf("exited, status=%d\n", WEXITSTATUS(status));
                            } else if (WIFSIGNALED(status)) {
                                printf("killed by signal %d\n", WTERMSIG(status));
                            } else if (WIFSTOPPED(status)) {
                                printf("stopped by signal %d\n", WSTOPSIG(status));
                            } else if (WIFCONTINUED(status)) {
                                printf("continued\n");
                            }
                        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
                        exit(EXIT_SUCCESS);
                        jobs.erase(jobs.end());
                    }
            }

        }
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
            perror("smash error: > fork has failed");
            exit(1);
        	case 0 :
                	// Child Process
               		setpgrp();
                    //
                    execvp(args[0], args);
                    perror("smash error: > couldn't run external command");
                    exit(1);
        default:
            int pross_status;
            // Add your code here
            Job fg_command(pID,jobs_counter++,static_cast<unsigned int>(time(NULL)),args[0],"FRONT");
            jobs.push_back(fg_command);
            int wait_result = waitpid(pID, &pross_status, WSTOPPED); //wait for the proccess to end.
            if (wait_result == -1 && WIFSTOPPED(pross_status)) {
                perror("smash error: > wait has failed");
                return;
            }
	}
    return;
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
int BgCmd(char* lineSize)
{

	char* Command;
	char delimiters[4] = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
        
		lineSize[strlen(lineSize)-2] = '\0';
        Command = strtok(lineSize, delimiters);
        if (Command == NULL){
            return 0;
        }
        for (int i = 1; i < MAX_ARG; i++)
        {
            args[i] = strtok(NULL, delimiters);
        }
        if ((!strcmp(Command, "diff")) || (!strcmp(Command, "kill")) || (!strcmp(Command, "quit")) || (!strcmp(Command, "fg")) || (!strcmp(Command, "bg")) || (!strcmp(Command, "showpid"))
            || (!strcmp(Command, "pwd")) || (!strcmp(Command, "jobs")) || (!strcmp(Command, "cd")))
        {
            perror("smash error: > can't run built-in commands in bg");
            return 0;
        }
        else{
            int pID;
            switch (pID = fork())
            {
            case -1:
                perror("smash error: > ");
                exit(1);
            case 0:
                // Child Process - implement difference between path and local file
                setpgrp();
                execvp(args[0], args);
                perror("smash error: > ");
                exit(1);
            default:
                Job bg_command(pID,jobs_counter++,static_cast<unsigned int>(time(NULL)),args[0],"BACK");
                jobs.push_back(bg_command);
                return 0;
            }
        }
        return -1;
    }
}

