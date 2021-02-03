#include <stdio.h>
#include <pthread.h>

int X;
int glob_var = 0;
int rc;
int interval = 15;
int trigger = 3;

//pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* square(void*arg)
{
  int num = 0;
  printf("Square thread(%u) started\n", pthread_self());    
  while ( rc >= 0 ) {
	if (num == glob_var && trigger != 1 ) sleep(interval) ; 
	else { 
		//pthread_mutex_lock(&mutex);
		printf("Square thread> computing x2...\n");		
		X = glob_var * glob_var;
		num = glob_var;
		trigger = 2;
		//pthread_mutex_unlock(&mutex);
	}
  }
    printf("Square thread> exiting\n"); 
    pthread_exit(NULL);
}


void *mul(void*arg){
  int num = 0;
  printf("Multiply thread(%u) started\n", pthread_self());    
  while ( rc >= 0 ) {
	if (num == glob_var && trigger != 2) sleep(interval) ; 
	else { 
		//pthread_mutex_lock(&mutex);
		printf("Multiply thread> computing 2x2...\n");
		X = X * 2;
		num = glob_var;
		trigger = 0;
		//pthread_mutex_unlock(&mutex);
	}
  }
    printf("Multiply thread> exiting\n"); 
    pthread_exit(NULL);
}


int main(int argc, char**argv){

	pthread_t tid1,tid2;
	
	if (pthread_create(&tid1, NULL, square, NULL)!= 0 )
        { perror("pthread_create"); return(1); }
	if (pthread_create(&tid2, NULL, mul, NULL)!= 0 )
        { perror("pthread_create"); return(1); }
	
	do{
		flockfile(stdout);
		printf("\nmain> Enter num or `ctrl+d` to end\n");
		rc = scanf("%d", &glob_var);
		funlockfile(stdout);
		if(rc == EOF)
			{printf("breaking the loop\n");break;}
		X = glob_var;
		trigger = 1;
		while(rc != EOF && trigger != 0 && X == glob_var){}
		if(rc != EOF)
			{printf("\nmain>  2x2 = %d\n", X);trigger = 1;}

	} while ( rc != EOF) ;

	pthread_join(tid1, NULL); 
	pthread_join(tid2, NULL); 
}