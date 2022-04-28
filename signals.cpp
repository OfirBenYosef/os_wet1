// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers
/*******************************************/
#include "signals.h"
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
            cout << "smash: process < "<< fg_job.pid <<" > was killed" <<endl;
            fg_job.pid = 0;
            fg_job.job_id = 0;
            fg_job.elp_sec = 0;
            strcpy(fg_job.command," ");
            strcpy(fg_job.status," ");
            fg_job.stop = false;
        }
    }
}
void ctrl_Z_sig_handler(int signal_num)
{
    cout<< "caught ctrl-Z" <<endl;
    int job_to_stop = jobs_counter;
    if (fg_job.pid == 0 ){
        return;
    }
    else{
        if(kill(fg_job.pid ,SIGSTOP) == -1){
            perror("smash error:");
            return;
        }
        fg_job.job_id == 0 ? jobs_counter++ : fg_job.job_id;
        fg_job.stop=true;
        strcpy(fg_job.status,"stopped");
        int pos_in_jobs;
        for(int i = 0; i<jobs.size();i++){
            if(fg_job.job_id >= jobs[i].job_id){
                pos_in_jobs = i;
            }
        }
        jobs.insert(jobs.begin()+ pos_in_jobs,fg_job);
        cout << "smash: process <"<< fg_job.pid <<" > was stopped" <<endl;
        fg_job.pid = 0;
        fg_job.job_id = 0;
        fg_job.elp_sec = 0;
        strcpy(fg_job.command," ");
        strcpy(fg_job.status," ");
        fg_job.stop = false;
        }
    return;
}
