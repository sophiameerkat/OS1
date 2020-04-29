#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sched.h>
#include "info.h"

int cmp (const void* a, const void* b){
	Pro *tmpa = (Pro *)a;
	Pro *tmpb = (Pro *)b;
	int ready_t1 = tmpa -> ready_time;
	int ready_t2 = tmpb -> ready_time;
	int id1 = tmpa -> id;
	int id2 = tmpb -> id;
	if (ready_t1 < ready_t2)
		return -1;
	else if (ready_t1 > ready_t2)
		return 1;
	else if (ready_t1 == ready_t2) {
		if (id1 < id2)
			return -1;
		else if (id1 > id2)
			return 1;
	}
	return 0;
}

int assign (int core_id, int pid) {
	if (core_id < CPU_SETSIZE){
		cpu_set_t s;
		CPU_ZERO(&s);
		CPU_SET(core_id, &s);
		if (sched_setaffinity(pid, sizeof(s), &s) == -1)
			fprintf(stderr, "CPU affinity error\n");
	}
	return 0;
}

int execute (Pro p) {
	int process;
	process = fork();
	if (process < 0)
		fprintf(stderr, "fork child process error\n");
	else if (process == 0) {
		struct timespec s, e;
		syscall(334, &s);
		for (int i = 0; i < p.exec_time; i++) {
			volatile unsigned long j; 
			for(j = 0; j < 1000000UL; j++); 
		}
		syscall(334, &e);
		int n = getpid();
		syscall(335, s.tv_sec, s.tv_nsec, e.tv_sec, e.tv_nsec, n);
		exit(0);
	}
	else 
		assign(1, process);
	return process;
}

int block (int pid) {
	//fprintf(stderr, "blockpid:%d\n", pid);
	//fflush(stderr);
	struct sched_param parameter;
	parameter.sched_priority = 1;
	return sched_setscheduler(pid, SCHED_FIFO, &parameter);
}

int wake (int pid) {
	//fprintf(stderr, "wakepid:%d\n", pid);
	//fflush(stderr);
	struct sched_param parameter;
	parameter.sched_priority = 99;
	return sched_setscheduler(pid, SCHED_FIFO, &parameter);
}
