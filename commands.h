#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define TRUE 1
#define FALSE 0
//********************************************************************
class Job{
        public:
        unsigned int pid;
        unsigned int job_id;
        unsigned int elp_sec;
        //char* command=(char*)malloc(80*sizeof(char));
        //char* status=(char*)malloc(80*sizeof(char));
        char command[MAX_LINE_SIZE];
        char status[MAX_LINE_SIZE];
        void Jprint();
        Job(unsigned int pid,unsigned int job_id,unsigned int elp_sec,char* command,char* status);
        Job();
};


extern std::vector<Job> jobs;
extern Job fg_job;
//********************************************************************
//typedef enum  { FALSE , TRUE }; bool;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize);
int ExeCmd(char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
void change_fg_job(Job fg,unsigned int pid,unsigned int job_id,unsigned int elp_sec,char* command,char* status);
int delete_finished_jobs();
#endif

