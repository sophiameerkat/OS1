#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/times.h>
#include <sched.h>
#include "info.h"

static int time_num = 0;
static int running_index = -1;
static int finish_num = 0;

void FIFO (Pro *p, int num_process) {
	qsort(p, num_process, sizeof(Pro), cmp);

	int cur_pid = getpid();
	assign(0, cur_pid);

	int exec_id = 0;
	while (finish_num < num_process) {
		if (running_index != -1) {
			int curexec = p[running_index].exec_time;
			if (curexec == 0) {
				waitpid(p[running_index].pid, NULL, 0);
				printf("%s %d\n", p[running_index].name, p[running_index].pid);
				fflush(stdout);
				finish_num += 1;
				running_index = -1;
			}
		}
		for (int i = exec_id; i < num_process; i++) {
			if (p[i].ready_time == time_num) {
				exec_id = i + 1;
				p[i].pid = execute(p[i]);
				block(p[i].pid);
			}
			else 
				break;
		}
		int nex;
		if (running_index != -1)
			nex = running_index;
		else {
			if (finish_num + 1 <= exec_id)
				nex = finish_num;
			else
				nex = -1;
		}
		if (nex != -1 && running_index != nex) {
			wake(p[nex].pid);
			block(p[running_index].pid);
			running_index = nex;
		}
		volatile unsigned long j; 
		for(j = 0; j < 1000000UL; j++);
		if (running_index != -1) 
			p[running_index].exec_time -= 1;
		time_num += 1;
	}
}
