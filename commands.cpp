//        commands.cpp

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
    if (stop){
        cout <<"["<<job_id<<"]"<<command<<" : "<< pid << " " << difftime(time(NULL), elp_sec)<<" secs"<<" (Stopped)"<< endl;
        }
   else if (!stop){
   // else{
        cout <<"["<<job_id<<"]"<<command<<" : "<< pid << " " << difftime(time(NULL), elp_sec)<<" secs"<< endl;
   }
};





Job::Job(unsigned int pid,unsigned int job_id,unsigned int elp_sec,char* command,char* status,bool stop){
            this->pid = pid;
            this->job_id = job_id;
            this->elp_sec = elp_sec;
            strcpy(this->command,(char*)command);
            strcpy(this->status,status);
            this->stop = stop;
};

Job::Job(){
         this->pid = 0;
         this->job_id = 0;
         this->elp_sec = 0;
         strcpy(this->command," ");
         strcpy(this->status," ");
         this->stop = false;
};





void change_fg_job(Job &fg,unsigned int pid,unsigned int job_id,unsigned int elp_sec,char* command,char* status,bool stop){
    fg.pid = pid;
    fg.job_id = job_id;
    fg.elp_sec = elp_sec;
    strcpy(fg.command,(char*)command);
    strcpy(fg.status,status);
    fg.stop = stop;
}

//**************************************************************************************

char OLDPWD[MAX_LINE_SIZE];

int jobs_counter = 1;

Job fg_job;



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
    delete_finished_jobs();
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
               else if (!strcmp(args[1], "-"))
                {
                    illegal_cmd = false;
                    if (OLDPWD[0] != 0)
                    {
                        char curr_dir[MAX_LINE_SIZE];
                        if (getcwd(curr_dir, MAX_LINE_SIZE) == NULL)
                        {
                            perror("smash error: ");
                            return -1;
                        }
                        if (chdir(OLDPWD) == -1)

                        {
                            perror("smash error: cd: OLDPWD not set");
                            return -1;
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
                        return -1;
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
        //delete_finished_jobs();
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

                 //printf("Error : Files not open");
                 //exit(0);
                 perror("smash error:");
                 return -1;
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
        vector<Job>::iterator ptr;
        if(num_arg > 1){
            perror("smash error: fg: invalid arguments");
            illegal_cmd = true;
        }
        else {
            if(num_arg == 1){
                for(ptr = jobs.begin(); ptr != jobs.end(); ptr++){
                   
                    if(ptr->job_id == atoi(args[1])){
                        illegal_cmd = false;
                        break;
                    }
                    illegal_cmd = true;
                }
                if(illegal_cmd){
                    cout << "smash error: fg:job-id " << args[1]<< " does not exist"<< endl;
                }
            }
            else if(num_arg == 0){
                if(!jobs.size()) {
                    perror("smash error: fg: jobs list is empty");
                    illegal_cmd = true;
                }
                else{ 
                    ptr = jobs.end();
		    ptr--;
		    while(ptr->stop){
			ptr--;
		    }
                    illegal_cmd = false;
                }
            }
            
            if(!illegal_cmd){
                fg_job.pid = ptr->pid;
                fg_job.job_id = ptr->job_id;
                fg_job.elp_sec = ptr->elp_sec;
                strcpy(fg_job.command,ptr->command);
                strcpy(fg_job.status,"FRONT");
                fg_job.stop = false;
                pid_t pID = ptr->pid;
                int val_stat;
                int wait_result = waitpid(pID, &val_stat, WSTOPPED);
                if (wait_result == -1 && WIFSTOPPED(val_stat))
                {
                    perror("smash error: > wait has failed");
                    return 1;
                }
                if (wait_result > 0 && WIFEXITED(val_stat))
                {
                    jobs.erase(ptr);
                    fg_job.pid = 0;
                    fg_job.job_id = 0;
                    fg_job.elp_sec = 0;
                    strcpy(fg_job.command," ");
                    strcpy(fg_job.status," ");
                    fg_job.stop = false;
                }
            }

        }

        
    }

    /*************************************************/

    else if (!strcmp(cmd, "bg"))

    {
        vector<Job>::iterator it;
        if(num_arg > 1){ //maybe need to add more conditions
            perror( "smash error: bg: invalid arguments");
	     illegal_cmd = false;
        }
        else {
            if (num_arg == 1) {
                if (!jobs.size()) {
 		  illegal_cmd = false;

                }
                else{
                    int i = 0;
                    for (vector<Job>::iterator it = jobs.begin(); it != jobs.end(); it++) {
                        
                        if (it->job_id + 48 == (int) *args[1]) {
                            if (it->stop) {
                                strcpy( it->status ,"");
                                it->stop = false;
                                it->Jprint();
                                int kill_result = kill(it->pid, SIGCONT); // continue the proccess.

                                if (kill_result == -1)
                                {
                                    perror("smash error: >  kill has failed");
                                    return 1;
                                }
                            }
                            else {
                                cout << "smash error: bg:job-id " << it->job_id << " is already running in the background"
                                     << endl;
                            }
                            return 0;
                        }
                    }
                        cout << "smash error: bg:job-id " << *args[1] << " does not exist" << endl;
                }

                
            }
            else if (num_arg == 0) {
                for (vector<Job>::iterator it = jobs.end(); it != jobs.begin();) {
                    it--;
                    if (it->stop) {
                        strcpy( it->status ,"");
                        it->stop = false;
                        
                        int kill_result = kill(it->pid, SIGCONT); // continue the proccess.

                        if (kill_result == -1)
                        {
                            perror("smash error: >  kill has failed");
                            return 1;
                        }
           
                        return 0;
                    }
                 
                }
              
                    cout << "smash error: bg:there are no stopped jobs to resume" << endl;               
            }
        }
        return 0;
    }

    /*************************************************/

    else if (!strcmp(cmd, "quit"))

    {
       if(num_arg == 1 && !strcmp(args[1], "kill")){
            //send SIGTERM to all job in jobs
           vector<Job>::iterator it_k = jobs.begin();
           for (; it_k != jobs.end(); it_k++){
               cout << "[" << it_k->job_id << "] " << it_k->command << " - Sending SIGTERM... ";
		fflush(stdout);
               if (kill(it_k->pid, SIGTERM) == -1) {
                   perror("smash error: >  kill has failed");
                   return 1;
               }
               else{
                   int kill_start_time = time(NULL);
                   int wait = 0;
                   while (difftime(time(NULL), kill_start_time) < 5) {
                       wait = waitpid(it_k->pid, NULL, WNOHANG);
                       if (wait == -1) {
                           perror("smash error: >  wait has failed");
                           return 1;
                       }
                       else if (wait > 0) {
                           break;
                       }
                       else {
                           continue;
                       }
                   }
                   if(wait == 0){
                       if (kill(it_k->pid, SIGKILL) == -1) {
                           perror("smash error: >  kill has failed");
                           return 1;
                       }
                       else{
                           cout << "(5 sec passed) Sending SIGKILL... Done." <<endl;
                       }
                   }
                   else{
                       cout << "Done." <<endl;
                   }
                   
               }
            }
        }
       vector<Job>::iterator it = jobs.begin();

	 for(int i = 0; i<jobs.size();i++){
                jobs.erase(jobs.begin() + i);                     
         }
	
       /*for (; it != jobs.end(); it++){
            jobs.erase(it);
        }*/
       exit(0);
           
    }
    else if (!strcmp(cmd, "kill"))

    {
        if(num_arg !=2 || args[1][0] != '-' || !is_string_digits(args[1], 1) || !is_string_digits(args[2])){
            perror("smash error: kill: invalid arguments");
            illegal_cmd = true;
        }
        else{
            int signum = atoi(args[1] + 1);
            //cout << "signum " << signum <<endl;
            vector<Job>::iterator it = jobs.begin();
           for (; it != jobs.end(); it++){
                if (it->job_id == atoi(args[2]))
                {
                    cout << "signal " << signum << " was sent to pid " << it->pid << endl;
                    if (kill(it->pid, signum) == -1)
                    {
                        cout << "smash error: > kill " << args[2] << " - cannot send signal" << endl;
                        return 1;
                    }
                    return 0;
                }

            }
            cout << "smash error: kill: job-id <" << atoi(args[2]) << "> does not exist" <<endl;
        }
        return 1;

    }

    /*************************************************/

    else // external command
    {
         ExeExternal(args, cmdString);
         delete_finished_jobs();
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
       pid_t pID;
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
           //  fg_job(pID,jobs_counter++,static_cast<unsigned int>(time(NULL)),args[0],"FRONT");
             char front[MAX_LINE_SIZE] ="FRONT";
          //  change_fg_job(fg_job,pID,0,static_cast<unsigned int>(time(NULL)),args[0],"FRONT",false);
           // jobs.push_back(fg_command);
            fg_job.pid = pID;
            fg_job.job_id = 0;
            fg_job.elp_sec = static_cast<unsigned int>(time(NULL));
            strcpy(fg_job.command,cmdString);
            strcpy(fg_job.status,front);
            fg_job.stop = false;
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
    delete_finished_jobs();
    char* Command = new char[MAX_LINE_SIZE];
    char* Com = new char[MAX_LINE_SIZE];

    char delimiters[4] = " \t\n";
    char *args[MAX_ARG];
    if (lineSize[(int)strlen(lineSize)-2] == '&')
    {
        lineSize[(int)strlen(lineSize)-2] = '\0';
        Command = strtok(lineSize, delimiters);
       // cout << "Command:" <<Command <<endl;
        if (Command == NULL){
		delete[] Com;
              //delete[] Command;
            return 0;
        }
        for (int i = 1; i < MAX_ARG; i++)
        {
            args[i] = strtok(NULL, delimiters);
        
        }
        args[0] = Command;
        if ((!strcmp(Command, "diff")) || (!strcmp(Command, "kill")) || (!strcmp(Command, "quit")) || (!strcmp(Command, "fg")) || (!strcmp(Command, "bg")) || (!strcmp(Command, "showpid"))
            || (!strcmp(Command, "pwd")) || (!strcmp(Command, "jobs")) || (!strcmp(Command, "cd")))
        {
            perror("smash error: > can't run built-in commands in bg");
               delete[] Com;
               delete[] Command;
               return 0;
        }
        else{
            pid_t pID = fork();
            if(pID < 0){
                perror("smash error: > ");
                delete[] Com;
                delete[] Command;
                exit(1);
            }
            else if( pID == 0){
                //cout << "pid == 0" <<endl;
                // Child Process - implement difference between path and local file
                setpgrp();
                if(execvp(args[0], args) == -1){
                perror("smash error: > ");
                delete[] Com;
                delete[] Command;
                exit(1);
                }
            }
            else{
                //cout << "pid > 0" <<endl;
                strcpy(Com,args[1]);
                Command = strcat(strcat(Command," "),Com);
                Command = strcat(Command," &");
                char back[MAX_LINE_SIZE] = "BACK";
                //Job bg_command(pID,jobs_counter++,static_cast<unsigned int>(time(NULL)),Command,back,false);
                    Job bg_command;
                    bg_command.pid = pID;
                    bg_command.job_id = jobs_counter++;
                    bg_command.elp_sec = static_cast<unsigned int>(time(NULL));
                    strcpy(bg_command.command,Command);
                    strcpy(bg_command.status,"BACK");
                    bg_command.stop = false;
                    jobs.push_back(bg_command);
                   //delete[] bg_command;
                    delete[] Com;
                  //delete[] Command;
            }

                return 0;
        //}
        }
        delete[] Com;
       // delete[] Command;
	return -1;
    }
	delete[] Com;
        //delete[] Command;	
    return 1;
}
int delete_finished_jobs()
{
    if (jobs.empty()) return 0;
    int stat_val;
    for (unsigned int i = 0; i < jobs.size(); i++)
    {
        int wait_result = waitpid((pid_t)jobs[i].pid, &stat_val, WNOHANG);
        if (wait_result == -1)
        {
            perror("smash error: > ");
            return 1;
        }
        else if ((wait_result > 0) && (WIFEXITED(stat_val) || WIFSIGNALED(stat_val)))
        {
            jobs.erase(jobs.begin() + i);//
            
        }
    }
    return 0;

}
bool is_string_digits(char* string, int start_index)
{
    int index = start_index;
    while (string[index])
    {
        if (string[index] < '0' || string[index] > '9') {
            return false;
        }
        index++;
    }
    return true;
}
