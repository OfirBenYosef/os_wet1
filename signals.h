#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"

void ctrl_C_sig_handler(int signal_num);
void ctrl_Z_sig_handler(int signal_num);
extern Job fg_job;
extern int jobs_counter;

#endif


