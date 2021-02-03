#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

FILE *pst1;
int on = 1;
int amount;
int account = 500;
int change;

sem_t s_account;

void *atm(void*arg)
{
	fprintf(pst1, "ATM thread(%u) started\n", pthread_self());    
	int num = 0;

	while ( ( on = scanf("%d", &amount ) ) != EOF) {
	 	sem_wait(&s_account);
	 		if(amount < account){
	 			account = account - amount;
				fprintf(pst1, "atm> %d dollar withdrawl from account: new amount: %d\n", amount, account);   
				change = account; 
	 		}
	 		else{
	 			account += amount;
				fprintf(pst1, "atm> %d dollar deposit from account: new amount: %d\n", amount, account); 

				change = account; 
	 		}
	 	sem_post(&s_account);
	 }
	 on = -1;

	printf ("\natm(%u)> terminating \n", pthread_self());

	return NULL;
}


void *display(void*arg)
{
	fprintf(pst1, "display thread(%u) started\n", pthread_self());    
	int num = 0;

	while ( on >= 0 ) {
		sleep(1);
		sem_wait(&s_account);
		if(account != change){
			fprintf(pst1, "display> account: %d\n", account);    
		}
		sem_post(&s_account);
	 }

	fprintf (pst1, "\ndisplay(%u)> terminating... account = %d \n", pthread_self(), account);
}

void *salary(void*arg){
	fprintf(pst1, "salary thread(%u) started\n", pthread_self());    
	int num = 0;

	while ( on >= 0 ) {
		sleep(10);
		sem_wait(&s_account);
			num = rand() % 1000 + 1000;
			account += num;
			fprintf(pst1, "salary> %d dollar deposit to account: new amount %d\n", num, account);
	 	sem_post(&s_account);    
	 }

	fprintf (pst1, "\nsalary(%u)> terminating account = %d \n", pthread_self(), account);
}
void *tax(void*arg){
	fprintf(pst1, "tax thread(%u) started\n", pthread_self());    
	int num = 0;

	while ( on >= 0 ) {
		sleep(2);
		sem_wait(&s_account);
			num = rand() % (account/10);
			account -= num;
			fprintf(pst1, "tax> %d dollar tax for the account...\n", num);    
	 	sem_post(&s_account);

	 }
	fprintf (pst1, "\ntax(%u)> terminating... account = %d \n", pthread_self(), account);
}

int main(){
	sem_init(&s_account, 0, 1);
	change = account;


	pthread_t displayTID, atmTID, salaryTID, taxTID;

	if ( ( pst1 = fopen ("/dev/pts/1", "w") ) == NULL )
	{ printf ("\n error messageâ€¦.."); return 0; }
	
	pthread_create(&atmTID, NULL, atm, NULL);
	pthread_create(&displayTID, NULL, display, NULL);
	pthread_create(&salaryTID, NULL, salary, NULL);
	pthread_create(&taxTID, NULL, tax, NULL);



	pthread_join (displayTID, NULL);
	pthread_join (atmTID, NULL);
	pthread_join (salaryTID, NULL);
	pthread_join (taxTID, NULL);

	printf("Main> terminating\n");
	return 0;
}

