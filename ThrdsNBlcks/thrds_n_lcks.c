/* Main thread of the process will block mutex and other locks (spinlock and rwlock), while other threads will try to lock them.
	Mutex and other locks can be released after process receives SIGUSR1 signal, thus allowing threads to lock them.
	Afterwards, threads themselves unlock mutex and other locks and are joined with main thread */

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>

pthread_mutex_t mut;
pthread_spinlock_t spin_lock;
pthread_rwlock_t readwrite_lock;


void * waiting_for_mutex(void * nothing_really_expected)
{
	pthread_mutex_lock(& mut);
	printf("Thread locked the mutex\n");
	pthread_mutex_unlock(& mut);
	printf("Thread unlocked the mutex\n");
	return NULL;
}

void * waiting_for_spinlock(void * nothing_really_expected)
{
	pthread_spin_lock(& spin_lock);
	printf("Thread locked the spinlock\n");
	pthread_spin_unlock(& spin_lock);
	printf("Thread unlocked the spinlock\n");
	return NULL;
}

void * waiting_for_read_or_write(void * mode)
{
	// printf("%s\n", (char*) mode);
	if (strcmp((char *)mode, "read") == 0)
		pthread_rwlock_rdlock(& readwrite_lock);
	else if (strcmp((char *)mode, "write") == 0) 
		pthread_rwlock_wrlock(& readwrite_lock);

	printf("Thread locked the %s lock\n", (char *)mode);
	pthread_rwlock_unlock(& readwrite_lock);
	printf("Thread unlocked the %s lock\n", (char *)mode);
	return NULL;
}

void sigusr1_handler(int sig_num)
{
	printf("Process received '%d' signal - time to release all mutexes and locks\n", sig_num);

	pthread_mutex_unlock(& mut);
	pthread_spin_unlock(& spin_lock);
	pthread_rwlock_unlock(& readwrite_lock);
}

int main(int argc, char const * argv[])
{
	printf("Process %d creates multiple blocks on threads and awaits SIGUSR1 signal to release them\n", getpid());
	pthread_t thr_id[4];

	int fd = open("main.pid", O_WRONLY | O_CREAT | O_TRUNC, 0666);
	char pid_str[sizeof(int)];
	sprintf(pid_str, "%d", getpid());
	write(fd, pid_str, sizeof(pid_str));

	// release locked objects after receiving SIGUSR signal
	signal(SIGUSR1, sigusr1_handler);

	// Mutex thread
	if (pthread_mutex_init(& mut, NULL) != 0)
	{
		perror("mutex_init");
		exit(1);
	}

	if (pthread_mutex_lock(& mut) != 0)
	{
		perror("mutex_lock");
		exit(1);
	}

	pthread_create(&thr_id[0], NULL, waiting_for_mutex, NULL);

	// Spin lock thread
	if (pthread_spin_init(& spin_lock, PTHREAD_PROCESS_SHARED) != 0)
	{
		perror("spin_init");
		exit(1);
	}

	if (pthread_spin_lock(& spin_lock) != 0)
	{
		perror("spin_lock");
		exit(1);
	}

	pthread_create(&thr_id[1], NULL, waiting_for_spinlock, NULL);

	// Read-write lock (single one for two threads)
	if (pthread_rwlock_init(& readwrite_lock, NULL) != 0)
	{
		perror("readwrite_lock_init");
		exit(1);
	}

	if (pthread_rwlock_wrlock(& readwrite_lock) != 0) /* chose write lock as
													we can actually read from something which was already blocked for reading */
	{
		perror("write lock on readwrite_lock");
		exit(1);
	}

	const char * mode_read = "read";
	const char * mode_write = "write";
	pthread_create(&thr_id[2], NULL, waiting_for_read_or_write, (void *) mode_read);
	pthread_create(&thr_id[3], NULL, waiting_for_read_or_write, (void *) mode_write);

	int i;
	for (i = 0; i < 4; ++i)
	{
		pthread_join(thr_id[i], NULL);
	}

	printf("All additional threads were joined\n");

	return 0;
}