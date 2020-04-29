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

static int time_num = 0, running_index = -1, finish_num = 0, last_time;

int queue[10000000] = {-2};
int head = 0, tail = 0;

int empty () {
	return head == tail;
}

void push (int additem) {
	queue[tail] = additem;
	tail++;
}
void pop () {
	if (empty()) {
		fprintf(stderr, "queue is empty\n");
		return;
	}
	queue[head] = -1;
	head++;
}

int front () {
	return queue[head];
}

void RR (Pro *p, int num_process) {
	qsort(p, num_process, sizeof(Pro), cmp);

	for (int i = 0; i < num_process; i++)
		p[i].pid = -1;

	int cur_pid = getpid();
	assign(0, cur_pid);

	int exec_id = 0;
	int tt = 1;
	while (finish_num < num_process) {
		if (running_index != -1) {
			int curexec = p[running_index].exec_time;
			if (curexec == 0) {
				waitpid(p[running_index].pid, NULL, 0);
				printf("%s %d\n", p[running_index].name, p[running_index].pid);
				fflush(stdout);
				finish_num += 1;
				running_index = -1;
				pop();
				tt = 1;
			}
		}
		for (int i = exec_id; i < num_process; i++) {
			if (p[i].ready_time == time_num) {
				exec_id = i + 1;
				p[i].pid = execute(p[i]);
				block(p[i].pid);
				push(i);
			}
			else 
				break;
		}
		int nex = -1;
		if (running_index == -1) {
			if (!empty())
				nex = front();
		}
		else {
			int f = front();
			if (tt == 5) {
				push(f);
				pop();
				nex = front();
				tt = 1;
			}
			else {
				tt += 1;
				nex = running_index;
			}
		}
		if (nex != -1 && running_index != nex) {
			wake(p[nex].pid);
			if (running_index != -1)
				block(p[running_index].pid);
			last_time = time_num;
			running_index = nex;
		}
		volatile unsigned long j; 
		for(j = 0; j < 1000000UL; j++);
		if (running_index != -1) 
			p[running_index].exec_time -= 1;
		time_num += 1;
	}
}
