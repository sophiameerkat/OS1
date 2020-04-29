all: main
main: main.c info.c info.h FIFO.c RR.c SJF.c PSJF.c
	gcc -o main main.c info.c FIFO.c RR.c SJF.c PSJF.c
clean: rm -f main
