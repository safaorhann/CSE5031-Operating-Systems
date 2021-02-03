//1600003764 --- SAFA ORHAN
//1600002640 --- BATUHAN UÃ‡SU
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int X;
int glob_var = 0;
int rc;
int interval = 10;
int trigger = 3;

sem_t s_main, s_sq, s_mul;


void* square(void*arg)
{
  	printf("Square thread(%u) started\n", pthread_self());    
	int num = 0;

	sem_wait(&s_main);
	  	printf("Square thread(%u) started computation\n", pthread_self());    
	  	sleep(interval);
		num = X;
		num = num * num;
		X = num;
	  	printf("Square thread(%u) computed value: %d\n", pthread_self(), X);    
  	sem_post(&s_sq);

  	printf("Square thread(%u) terminating\n", pthread_self());    
    pthread_exit(NULL);
}


void *mul(void*arg)
{
  	printf("Multiply thread(%u) started\n", pthread_self());    
	int num = 0;

	sem_wait(&s_sq);
	  	printf("Multiply thread(%u) started computation\n", pthread_self());    
	  	sleep(interval);
		num = X;
		num = 2 * num;
		X = num;
	  	printf("Multiply thread(%u) computed value: %d\n", pthread_self(), X); 
  	sem_post(&s_mul);

  	printf("Multiply thread(%u) terminating\n", pthread_self());   

    pthread_exit(NULL);
}


int main(int argc, char**argv){

	sem_init(&s_main, 0, 0);
	sem_init(&s_sq, 0, 0);
	sem_init(&s_mul, 0, 0);

	pthread_t tid[2];
	
	if (pthread_create(&tid[0], NULL, square, NULL)!= 0 )
    { perror("pthread_create"); return(1); }
    printf("Main thread> created the thread: %u\n", tid[0]);
    if (pthread_create(&tid[1], NULL, mul, NULL)!= 0 )
    { perror("pthread_create"); return(1); }
    printf("Main thread> created the thread: %u\n", tid[1]);

    //printf("please enter a number for calculation\n");
	rc = scanf("%d", &X);

	sem_post(&s_main);
	sem_wait(&s_mul);

	printf("Main thread> calculated value: %d\n", X);

	pthread_join(tid[0], NULL); 
	pthread_join(tid[1], NULL); 

	printf("Main thread> terminating...\n");

	return 0;
}