#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include "info.h"

int main(int argc, char** argv) {
	char scheduling_policy[128];
	int num_process;

	scanf("%s", scheduling_policy);
	scanf("%d", &num_process);

	Pro *p;
	p = (Pro *)malloc(num_process * sizeof(Pro));

	for (int i = 0; i < num_process; i++) {
		scanf("%s %d %d", p[i].name, &p[i].ready_time, &p[i].exec_time);
		p[i].id = i;
	}
	
	if (strcmp(scheduling_policy, "FIFO") == 0)
		FIFO(p, num_process);
	else if (strcmp(scheduling_policy, "RR") == 0)
		RR(p, num_process);
	else if (strcmp(scheduling_policy, "SJF") == 0)
		SJF(p, num_process);
	else if (strcmp(scheduling_policy, "PSJF") == 0)
		PSJF(p, num_process);
	else 
		fprintf(stderr, "policy type error\n");
	return 0;
}
