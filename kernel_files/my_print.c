#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_my_print(int ss, int sn, int es, int en, int pid) {
	printk(KERN_INFO "[Project1] %d %d.%09d %d.%09d\n", pid, ss, sn, es, en);
}
