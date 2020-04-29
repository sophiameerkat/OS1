#ifndef _SCHEDULING_H_
#define _SCHEDULING_H_
#include <sys/types.h>

typedef struct pro {
	char name[32];
	int ready_time;
	int exec_time;
	pid_t pid;
	int id;
}Pro;

void FIFO(Pro *p, int num_process);
void RR(Pro *p, int num_process);
void SJF(Pro *p, int num_process);
void PSJF(Pro *p, int num_process);
int cmp (const void* a, const void* b);
int assign (int core_id, int pid);
int execute (Pro p);
int block (int pid);
int wake (int pid);
int wake (int pid);

#endif
