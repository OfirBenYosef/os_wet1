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
    //delete_finished_jobs();
/*************************************************/

// Built in Commands PLEASE NOTE NOT ALL REQUIRED

// ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD

// MORE IF STATEMENTS AS REQUIRED

/*************************************************/

    if (!strcmp(cmd, "cd") )

    {
        //delete_finished_jobs();
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
        delete_finished_jobs();
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
        delete_finished_jobs();
    }

    /*************************************************/

    else if (!strcmp(cmd, "diff"))

    {
        //delete_finished_jobs();
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
        delete_finished_jobs();
    }

    /*************************************************/

    

    /*************************************************/

    else if (!strcmp(cmd, "mkdir"))

    {

         

    }

    /*************************************************/

    else if (!strcmp(cmd, "jobs"))

    {
        //delete_finished_jobs();
        vector<Job>::iterator it =jobs.begin();
        while(it != jobs.end()) {
            it->Jprint();
            it++;
        }
        delete_finished_jobs();
    }

    /*************************************************/

    else if (!strcmp(cmd, "showpid"))

    {
       // delete_finished_jobs();
        (num_arg != 0)? (illegal_cmd = true) : (illegal_cmd = false);
        if (!illegal_cmd)
        {
            cout << "smash pid is " << getpid() << endl;
        }
        delete_finished_jobs();
    }
    
    /*************************************************/

    else if (!strcmp(cmd, "fg"))

    {
        //delete_finished_jobs();
        vector<Job>::iterator ptr ;
	


        if(num_arg > 1){ //maybe need to add more conditions

            cout << "smash error: fg: invalid arguments" << endl;
            //perror("smash error:");
           // return -1;
        }

        else{

            if(num_arg == 1){
                if(!jobs.size()){
                    cout << "smash error: fg:job-id " << *args[1]<< " does not exist"<< endl;
                }
                int i = 0;
                for(vector<Job>::iterator ptr = jobs.begin(); ptr != jobs.end(); ptr++)
                {
                    if(ptr->job_id+48== (int)*args[1]){
                        do {
                            w = waitpid((pid_t)ptr->pid, &status, WUNTRACED | WCONTINUED);

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
                        jobs.erase(ptr);
                        return 0;
                    }
                }

                
                    cout << "smash error: fg:job-id " << *args[1] << " does not exist" << endl;
                
            }
            else if(num_arg == 0){
                    if(!jobs.size()) {
                        cout << "smash error: fg: jobs list is empty" << endl;
                    }
                    else{
                        ptr = jobs.end();
                        ptr--;
                        do {
                            w = waitpid((pid_t)ptr->pid, &status, WUNTRACED | WCONTINUED);

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
                        //exit(EXIT_SUCCESS);
                        jobs.erase(ptr);
                    }
            }
        }
        //delete_finished_jobs();
	return 0;
    }

    /*************************************************/

    else if (!strcmp(cmd, "bg"))

    {
        //delete_finished_jobs();
        vector<Job>::iterator it;
        if(num_arg > 1){ //maybe need to add more conditions
            cout << "smash error: bg: invalid arguments" << endl;
        }
        else {
            if (num_arg == 1) {
                if (!jobs.size()) {
                    cout << "smash error: fg:job-id " << *args[1] << " does not exist" << endl;
                }
                int i = 0;
                for (vector<Job>::iterator it = jobs.begin(); it != jobs.end(); it++) {
                    if (it->job_id + 48 == (int) *args[1]) {
                        if (!strcmp(it->status, "(stopped)")) {
                           strcpy( it->status ,"");
                        } else {
                            cout << "smash error: bg:job-id " << it->job_id << " is already running in the background"
                                 << endl;
                        }
                        return 0;
                    }
                }
                
                    cout << "smash error: bg:job-id " << *args[1] << " does not exist" << endl;
                
            }
            else if (num_arg == 0) {
                it = jobs.end();
                it--;
                jobs.erase(it);
                for (vector<Job>::iterator it = jobs.end(); it != jobs.begin();) {
                    it--;
                    if (!strcmp(it->status, "(stopped)")) {
                        strcpy( it->status ,"");
                        return 0;
                    }
                    else {
                        cout << "smash error: bg:job-id " << it->job_id << " is already running in the background"
                             << endl;
                        return 0;
                    }
                }
              
                    cout << "smash error: bg:there are no stopped jobs to resume" << endl;
                
            }
        }
        //delete_finished_jobs();
return 0;
    }

    /*************************************************/

    else if (!strcmp(cmd, "quit"))

    {
        delete_finished_jobs();
           

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
        delete_finished_jobs();
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
    //delete_finished_jobs();
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
            strcpy(fg_job.command,(char*)args[0]);
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

    char* Command=new char[MAX_LINE_SIZE];
    char* Com=new char[MAX_LINE_SIZE];

    char delimiters[4] = " \t\n";
    char *args[MAX_ARG];
    if (lineSize[(int)strlen(lineSize)-2] == '&')
    {
        lineSize[(int)strlen(lineSize)-2] = '\0';
        Command = strtok(lineSize, delimiters);
        if (Command == NULL){
		delete[] Com;
              //delete[] Command;
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
		delete[] Com;
               delete[] Command;
            return 0;
        }
        else{
            pid_t pID;
            switch (pID = fork())
            {
            case -1:
                perror("smash error: > ");
		delete[] Com;
                delete[] Command;
                exit(1);
            case 0:
                // Child Process - implement difference between path and local file
                setpgrp();
                execvp(args[0], args);
                perror("smash error: > ");
		delete[] Com;
                delete[] Command;	
                exit(1);
            
	default:   
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
		
        
                return 0;
            }
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

   /* if (jobs.empty()){
        return 0;
    }
    int stat_val;
    for (unsigned int i = 0; i < jobs.size(); i++)
    {
       // pid_t wait_result = waitpid(jobs[i].pid, &stat_val, WNOHANG | WUNTRACED | WCONTINUED);
        pid_t wait_result = waitpid(jobs[i].pid, &stat_val,  WNOHANG | WUNTRACED | WCONTINUED);
        //cout<<"jobs[i].pid "<<jobs[i].pid<<endl;
        cout<<"wait_result = "<< wait_result<<endl;
        if ((wait_result  0) && (WIFEXITED(stat_val) || WIFSIGNALED(stat_val)))
        {
        cout<<"delete pid = "<< jobs[i].pid <<endl;

            jobs.erase(jobs.begin() + i);

        }
    }
    return 0;*/

    if (jobs.empty()) return 0;
    int stat_val;
    for (unsigned int i = 0; i < jobs.size(); i++)
    {
        int wait_result = waitpid((pid_t)jobs[i].pid, &stat_val, WNOHANG);
        cout<<"wait_result = "<< wait_result<<endl;
        if (wait_result == -1)
        {
            perror("smash error: > ");
           // return 1;
        }
        else if (wait_result > 0) //&& (WIFEXITED(stat_val) || WIFSIGNALED(stat_val)))
        {
            jobs.erase(jobs.begin() + i);
            cout<<"delete pid = "<< jobs[i].pid <<endl;
        }
    }
    return 0;

}
