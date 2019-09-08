/* Process awaits SIGUSR1 signal to awake one thread waiting on condition variable,
   which in turn will awake another thread awaiting on barrier */

#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

pthread_mutex_t mut;
pthread_cond_t condition_variable;
pthread_barrier_t barrier;

void * condition_thread(void * expecting_null_here)
{
	pthread_mutex_lock(&mut);
	printf("Waiting for condition variable in thread with ID %ld\n", (long)pthread_self() % 1000);
	pthread_cond_wait(&condition_variable, &mut);
	printf("Thread %ld: waiting for condition variable is OVER!\n", (long)pthread_self() % 1000);
	pthread_barrier_wait(&barrier);

	return NULL;
}

void * barrier_thread(void * expecting_null_here)
{
	printf("Waiting for every thread reaching same barrier as thread with ID %ld\n", (long)pthread_self() % 1000);
	pthread_barrier_wait(&barrier);
	printf("Thread %ld: barrier was passed\n", (long)pthread_self() % 1000);
	return NULL;
}

void signal_handler(int sig_num)
{
	if (sig_num == SIGUSR1)
	{
		pthread_mutex_lock(&mut);
		pthread_cond_signal(&condition_variable); 
		pthread_mutex_unlock(&mut);
	}
}

int main(int argc, char const * argv[])
{
	if (pthread_mutex_init(&mut, NULL) != 0)
	{
		perror("mutex_init");
		exit(1);
	}

	if (pthread_cond_init(&condition_variable, NULL) != 0)
	{
		perror("cond_init");
		exit(1);
	}

	if (pthread_barrier_init(&barrier, NULL, 2) != 0)
	{
		perror("barrier_init");
		exit(1);
	}

	int fd;
	if ((fd = open("main.pid", O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
	{
		perror("open file descriptor");
		exit(1);
	}

	printf("Process ID is %d, main thread ID is %ld\n", (int)getpid(), (long)pthread_self() % 1000);
	char str[sizeof(int)];
	sprintf(str, "%d", (int)getpid());
	write(fd, str, sizeof(str));
	close(fd);

	printf("Signal SIGUSR1 will change value of condition variable to awake waiting thread\n");
	signal(SIGUSR1, signal_handler);

	pthread_t thr[2];
	
	pthread_create(&thr[0], NULL, condition_thread, NULL);
	pthread_create(&thr[1], NULL, barrier_thread, NULL);

	pthread_join(thr[0], NULL);
	pthread_join(thr[1], NULL);

	printf("Thread %ld: all 'worker' threads were joined\n", (long)pthread_self() % 1000);

	return 0;
}