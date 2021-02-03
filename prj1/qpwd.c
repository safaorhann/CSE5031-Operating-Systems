#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
#define MAXRLEN  1023  
#define MAXRNUM  100 

void foo(char *c){
	printf("%s\n", c);
   	int k = 0, count = 0;
   	int i = 0, j = 0;
	char *home = (char*)malloc(sizeof(char)* (strlen(c)+1));
	char *shll = (char*)malloc(sizeof(char)* (strlen(c)+1));

   	while(k < strlen(c))
   	{

   			if(count == 5 && c[k] != ':'){
   					home[i++] = c[k];
   			}
   			if(count == 6){
   					shll[j++] = c[k];
   			}

   			if(c[k] == ':'){
   				count++;
   			}
   			k++;   		
   	}
   	home[k] = '\0';
   	shll[k] = '\0';
	printf("home directory: %s\n", home);
	printf("login shell: %s\n", shll);
}
      

int main(int argc, char **argv)
{	
	char usrname[15];
	char *cptr = argv[0];
	char *cptr1 = getenv("USER");

	printf("The name of the query program is: %s, the user account: %s\n", cptr, cptr1);
	printf("Enter username: ");

	scanf("%s",&usrname);
	char SortTab [MAXRNUM+1] [MAXRLEN+1]; 
	int  SortIdx [MAXRNUM+1];

	int nbusr, i, j, tmp;
	FILE *fptst;		

	char *user = (char*) malloc(sizeof(char)*strlen(usrname));
	strcpy(user, usrname);
	
	if ((fptst= fopen("./tstpwd.srt", "r") ) == NULL)
		{ perror("./tstpwd: "); return 1; } 

	i = 0;    
	for ( nbusr= 0; nbusr < MAXRNUM ; nbusr++ ) {
		SortIdx[nbusr] = nbusr;  
		if (fgets(SortTab[nbusr], (MAXRLEN + 1), fptst) == NULL )
			break;
	}

	if (fclose(fptst)!= 0)
   			{perror(" tstpwd  "); return 5;}


   	int k = 0, q = 0;
   	int found = 0;
   	for(i = 0; i < nbusr; i++){
   		char *cmp = (char*)malloc(sizeof(char)* strlen(SortTab[i]));
   		while(SortTab[i][k] != ':'){
   			cmp[k] = SortTab[i][k];
   			k++;
   		}
   		cmp[k] = '\0';
   		k=0;
   		if(strcmp(user, cmp) == 0){
   			found = 1;
   			foo(SortTab[i]);
   		}
   		
   		cmp = NULL;
   	}
   	if(!found)
   		printf("\nnot found\n");

	return 0;
}