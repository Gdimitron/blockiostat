#define _BSD_SOURCE	// for wait3 support

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>

void print_usage(char *exec)
{
	printf("Usage:\n  %s <command> <args>\n", exec);
	printf("\nExample:\n  %s cp ./file_src ./file_dst\n", exec);
	printf("\nNote:\n  To check block size try: blockdev --getbsz <partition>\n"); 
}

void gettime(struct timeval *val)
{
	if (gettimeofday(val, NULL) == -1) {
		perror("gettimeofday");
                exit(1);
	}
}

double diff_sec(struct timeval *val_beg, struct timeval *val_end) 
{
	long sec_diff = val_end->tv_sec - val_beg->tv_sec;
	long usec_diff = val_end->tv_usec - val_beg->tv_usec;
	double dsec_diff = ((sec_diff) * 1000 + usec_diff / 1000.0) + 0.5;
	return dsec_diff;
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		print_usage(argv[0]);
		exit(1);
	}
	
	struct timeval t_beg, t_end;
	gettime(&t_beg);

	pid_t pid = fork();
       	if (pid == -1) {
		perror("fork");
		exit(1);
	}
	if (!pid) {
		int ret = execvp(argv[1], argv + 1);
		if (ret == -1) {
			perror("exec");
			exit(1);
		}
	}
	int status;
	struct rusage res_usage;
	pid_t child_pid = wait3(&status, 0, &res_usage);
	if (child_pid == -1) {
		perror("wait3");
		exit(1);
	}
	gettime(&t_end);
	double dsec_diff = diff_sec(&t_beg, &t_end);

	printf("\nInput blocks/sec: %f \nOutput blocks/sec: %f\n",
			res_usage.ru_inblock / dsec_diff,
			res_usage.ru_oublock / dsec_diff);
	return 0;		       
}
