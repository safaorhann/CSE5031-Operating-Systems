#include <stdio.h>
#include <pthread.h>

int X = 0;
int rc;
int interval = 10;
int glob_var = 0;

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* square(void*arg)
{
  int num = 0;
  printf("Square thread(%u) started\n", pthread_self());    
  while ( rc >= 0 ) {
	if (num == X ) sleep(interval) ; 
	else { 
		//pthread_mutex_lock(&mutex);
		printf("Square thread> computing x2...\n");
			glob_var = X * X;
			num = X;
		//pthread_mutex_unlock(&mutex);
	}
  }
    printf("Square thread> exiting\n"); 
    pthread_exit(NULL);
}

int main(int argc, char*argv[]){

	pthread_t tid;
	
	if (pthread_create(&tid, NULL, square, NULL)!= 0 )
        { perror("pthread_create"); return(1); }
	
	do{
		printf("\nmain> Enter num or `ctrl+d` to end\n");
		rc = scanf("%d", &X) ;
		if(rc == EOF)
			{printf("main> breaking the loop\n ");break;}
		glob_var = X;
		while(rc != EOF && glob_var == X);

		if(rc != EOF)
			{printf("\nmain>  square of X = %d\n", glob_var);}

	} while ( rc != EOF) ;

	pthread_join(tid, NULL); 
}