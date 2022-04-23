// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
//Job fg_job;
using namespace std;

void ctrl_C_sig_handler(int signal_num)
{
    cout<< "caught ctrl-C" <<endl;
    if(fg_job.pid == 0){
        return;
    }
    else{
        if(kill(fg_job.pid ,SIGINT) == -1){
            perror("smash error:");
            return;
        }
        else{
            cout << "smash: process < "<< fg_job.pid <<"> was killed" <<endl;
            int job_to_kill = fg_job.job_id;
            //find the job and remove it
            
        }
    }
  
}
void ctrl_Z_sig_handler(int signal_num)
{
    cout<< "caught ctrl-Z" <<endl;
    //find the job to stop
    int job_to_stop = fg_job.job_id;
    
    if (fg_job.job_id == 0 ){
        return;
    }
    else{
        if(kill(fg_job.pid ,SIGSTOP) == -1){
            perror("smash error:");
            return;
        }
        strcpy(jobs[job_to_stop].status,"stopped");
        cout << "smash: process <"<< fg_job.pid <<"> was stopped" <<endl;
        //clear fg_job
  
        }
    return;
}
