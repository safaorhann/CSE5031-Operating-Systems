#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>

int X, x1, x2;
int glob_var = 0;
int rc;
int interval = 10;

sem_t s_main, s_sq, s_mul, s_div, s_add, s_main1, s_main2;

struct param {
	int x1;
	int x2;
};


void* square(void*arg)
{
  	printf("Square thread(%u) started\n", pthread_self());    
	int num = 0;

	sem_wait(&s_main);
	  	printf("Square thread(%u) started computation\n", pthread_self());    
	  	sleep(interval);
		num = x1;
		num = num * num;
		x1 = num;
	  	printf("Square thread(%u) computed value: %d\n", pthread_self(), x1);    
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
		num = x1;
		num = 2 * num;
		x1 = num;
	  	printf("Multiply thread(%u) computed value: %d\n", pthread_self(), x1); 
  	sem_post(&s_mul);

  	printf("Multiply thread(%u) terminating\n", pthread_self());   

    pthread_exit(NULL);
}


void *div(void*arg)
{
  	printf("Division thread(%u) started\n", pthread_self());    
	int num = 0;

	sem_wait(&s_main1);
	  	printf("Division thread(%u) started computation\n", pthread_self());    
	  	sleep(interval);
		num = x2;
		num = num / 2;
		x2 = num;
	  	printf("Division thread(%u) computed value: %d\n", pthread_self(), x2); 
  	sem_post(&s_div);

  	printf("Division thread(%u) terminating\n", pthread_self());   

    pthread_exit(NULL);
}


void *add(void*arg)
{
  	printf("Addition thread(%u) started\n", pthread_self());    
	int num = 0;
	struct param *args = (struct param*) arg;
	sem_wait(&s_main2);
	  	printf("Addition thread(%u) started computation\n", pthread_self());    
	  	sleep(interval);
		num = args->x1 + args->x2;
		X = num;
	  	printf("Addition thread(%u) computed value: %d\n", pthread_self(), X); 
  	sem_post(&s_add);

  	printf("Addition thread(%u) terminating\n", pthread_self());   

    pthread_exit(NULL);
}

struct param par;


int main(){
	sem_init(&s_main, 0, 0);
	sem_init(&s_main1, 0, 0);
	sem_init(&s_main2, 0, 0);
	sem_init(&s_sq, 0, 0);
	sem_init(&s_mul, 0, 0);
	sem_init(&s_add, 0, 0);
	sem_init(&s_div, 0, 0);

	pthread_t tid[4];

	pthread_create(&tid[0], NULL, square, NULL);
    printf("Main> created the thread: %u\n", tid[0]);
	pthread_create(&tid[1], NULL, mul, NULL);
    printf("Main> created the thread: %u\n", tid[1]);
	pthread_create(&tid[2], NULL, div, NULL);
    printf("Main> created the thread: %u\n", tid[2]);

	//printf("enter a number\n");
	rc = scanf("%d", &X);
	x1 = X; x2 = X;

	sem_post(&s_main);
	sem_post(&s_main1);
	sem_wait(&s_mul);
	sem_wait(&s_div);

	par.x1 = x1;
	par.x2 = x2;

	pthread_create(&tid[3], NULL, add, &par);
    printf("Main> created the thread: %u\n", tid[3]);

	sem_post(&s_main2);
	sem_wait(&s_add);

	printf("Main> calculation done X: %d\n", X);

	int i;

	for(i=0; i<4; i++){
		pthread_join(tid[i], NULL);
	}

	printf("Main> terminating\n");

}

















//1600003764 --- SAFA ORHAN
//1600002640 --- BATUHAN UÃ‡SU
/*
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
int X;
int glob_var = 0;
int rc;
int interval = 5;
sem_t s_main, s_sq, s_mul, s_div, s_add;
struct param {
	int x1;
	int x2;
};

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


void *div(void*arg)
{
  	printf("Division thread(%u) started\n", pthread_self());    
	int num = 0;

	sem_wait(&s_main);
	  	printf("Division thread(%u) started computation\n", pthread_self());    
	  	sleep(interval);
		num = X;
		num = num / 2;
		X = num;
	  	printf("Division thread(%u) computed value: %d\n", pthread_self(), X); 
  	sem_post(&s_div);

  	printf("Division thread(%u) terminating\n", pthread_self());   

    pthread_exit(NULL);
}

void *add(void*arg)
{
  	printf("Addition thread(%u) started\n", pthread_self());    
	int num = 0;
	struct param *args = (struct param*) arg;
	sem_wait(&s_main);
	  	printf("Addition thread(%u) started computation\n", pthread_self());    
	  	sleep(interval);
		num = args->x1 + args->x2;
		X = num;
	  	printf("Addition thread(%u) computed value: %d\n", pthread_self(), X); 
  	sem_post(&s_add);

  	printf("Addition thread(%u) terminating\n", pthread_self());   

    pthread_exit(NULL);
}

struct param par;

int main(){

	int status;

	sem_init(&s_main, 0, 0);
	sem_init(&s_sq, 0, 0);
	sem_init(&s_mul, 0, 0);
	sem_init(&s_add, 0, 0);
	sem_init(&s_div, 0, 0);

	int pid = fork();
	int pid2;
	int pdf1[2], pdf2[2];

	pipe(pdf1);
	pipe(pdf2);

	rc = scanf("%d", &X);

	if(pid>0)//child1 pro
	{
		pthread_t tid[2];
		if (pthread_create(&tid[0], NULL, square, NULL)!= 0 )
	    printf("child1 > square created thread: %u\n", tid[0]);
	    if (pthread_create(&tid[1], NULL, mul, NULL)!= 0 )
	    printf("child1 > mul created thread: %u\n", tid[1]);
		sem_post(&s_main);
		sem_wait(&s_mul);
		close(pdf1[0]);
		close(pdf2[0]);
		close(pdf2[1]);
		write(pdf1[1], &X, sizeof(X));
		close(pdf1[1]);
		pthread_join(tid[0], NULL); 
		pthread_join(tid[1], NULL); 
	}
	else{
		pid2 = fork();
		if(pid2 > 0)//child2 pro
		{
			pthread_t tid;
			if (pthread_create(&tid, NULL, div, NULL)!= 0 )
	    	printf("child2 > square created thread: %u\n", tid);
			sem_post(&s_main);
			sem_wait(&s_div);
			close(pdf1[0]);
			close(pdf2[0]);
			close(pdf1[1]);
			write(pdf2[1], &X, sizeof(X));
			close(pdf2[1]);
			pthread_join(tid, NULL); 
		}
		else
		{
			while(wait(&status) > 0){}
			pthread_t tid;
			close(pdf1[1]);
			close(pdf2[1]);

			read(pdf1[0], &par.x1, sizeof(int));
			read(pdf2[0], &par.x2, sizeof(int));
			close(pdf1[0]);
			close(pdf2[0]);
			if (pthread_create(&tid, NULL, add, &par)!= 0 )
	    	printf("parent > square created thread: %u\n", tid);
			sem_post(&s_main);
			sem_wait(&s_add);
			printf("parent value>: %d\n", X);
			pthread_join(tid, NULL); 
	}}
}
*/
