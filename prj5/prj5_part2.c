#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

FILE *pst1;
int arr[4] = {1,2,3,4};
int arrindex = 0;
int workperiod = 10;
int adviceperiod = 5;
int coffeeperiod = 3;
int napperiod = 1;
int seat_occupied = 0;
int on = 1;
int active;

pthread_mutex_t lock; 

//fprintf (pst1, "\ntax(%u)> terminating... account = %d \n", pthread_self(), account);

sem_t s_chair, s_advice, s_ta, s_stu, s_waitta;

void * ta(void*arg){
	fprintf(pst1, "teaching assistant thread(%u) started\n", pthread_self()); 
	fprintf(pst1, "teaching assistant opened office, waiting for a student\n"); 
	while(active){
		sem_wait(&s_ta);
		fprintf(pst1, "teaching assistant started session\n");
		sem_post(&s_waitta);
		sem_wait(&s_stu);
		pthread_mutex_lock(&lock);
			seat_occupied--;
		pthread_mutex_unlock(&lock);
		fprintf(pst1, "teaching assistant done with advising, nap time\n");
		sleep(napperiod);
		sem_post(&s_advice);
	}
	fprintf(pst1, "ta> no more active students, terminating...\n");
}
void * stu(void*arg){

	int num = arr[arrindex++];

	fprintf(pst1, "student %d thread(%u) started, going to the lab...\n", num, pthread_self());

	while(on){

		fprintf(pst1, "student %d is starting programmig\n", num);
		sleep(workperiod);

		fprintf(pst1, "student %d is checking the ta office\n", num);

		if(seat_occupied == 0){
			pthread_mutex_lock(&lock);
				seat_occupied++;
			pthread_mutex_unlock(&lock);

			fprintf(pst1, "student %d took a chair\n", num);
			sem_post(&s_ta);

			fprintf(pst1, "student %d waking up the teaching assistant\n", num);
			sem_wait(&s_waitta);

			fprintf(pst1, "student %d getting advice from the teaching assistant\n", num);
			sleep(adviceperiod);

			fprintf(pst1, "student %d has no more questions, leaving ta's office\n", num);
			sem_post(&s_stu);

		}
		else if(seat_occupied == 1){
			pthread_mutex_lock(&lock);
				seat_occupied++;
			pthread_mutex_unlock(&lock);

			fprintf(pst1, "student %d took a chair\n", num);

			fprintf(pst1, "student %d waiting for other student to get advice\n", num);
			sem_wait(&s_advice);

			sem_post(&s_ta);
			fprintf(pst1, "student %d waking up the teaching assistant\n", num);
			
			sem_wait(&s_waitta);

			fprintf(pst1, "student %d getting advice from the teaching assistant\n", num);
			sleep(adviceperiod);

			fprintf(pst1, "student %d has no more questions, leaving ta's office\n", num);
			sem_post(&s_stu);

			sem_post(&s_advice);

		}
		else{
			fprintf(pst1, "student %d> chairs in ta's office occupied, coffee break\n", num);

			sleep(coffeeperiod);
			
			fprintf(pst1, "student %u returnin the lab to resume programmig\n", num);

		}
	}
	pthread_mutex_lock(&lock);
	--active;
	fprintf(pst1, "student %d> signing off... active students: %d\n", num, active);
	pthread_mutex_unlock(&lock);
}


int main(){

	pthread_mutex_init(&lock, NULL);

	sem_init(&s_ta, 0, 0);
	sem_init(&s_stu, 0, 0);
	sem_init(&s_advice, 0, 0);
	sem_init(&s_chair, 0, 0);

	pthread_t taTID, stuID[4];

	if ( ( pst1 = fopen ("/dev/pts/1", "w") ) == NULL )
	{ printf ("\n error messageÂ¦.."); return 0; }

	printf("enter how many students\n");
	int s;
	scanf("%d", &s);
	active = s;

	pthread_create(&taTID, NULL, ta, NULL);
	
	int i = 0;
	for(i = 0; i < s; i++){
		pthread_create(&stuID[i], NULL, stu, NULL);
	}

	printf("press 0 if you want to end student threads\n");
	scanf("%d", &on);

	for(i = 0; i < s; i++){
		pthread_join(stuID[i], NULL);
	}

	pthread_join(taTID, NULL);


	fprintf(pst1, "main> terminating\n");
	return 0;
}