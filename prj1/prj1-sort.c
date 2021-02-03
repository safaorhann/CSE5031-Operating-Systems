#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 

int actcmp(char *s, char *t)
{
	for( ; *s == *t ; s++, t++)
		if ( *s == ':') return 0; // s = t
	if ( *s == ':') return -1 ;   // s < t
	if ( *t == ':') return +1 ;   // s > t
	return *s - *t ;
}

int main(int argc, char **argv)
{	
	char ** sorttab;
	int *sortidx;

	int maxrnum = atoi(argv[1]);
	int maxrlen = atoi(argv[2]);

	sortidx = (int *) malloc((maxrnum + 1) * sizeof(int)); 
	sorttab = (char **)malloc((maxrnum + 1) * sizeof(char *));
	int k = 0;
	for(k=0; k< maxrnum; k++)	{

    	sorttab[k] = (char *)malloc((maxrlen+1) * sizeof(char));

	}

	int nbusr, i, j, tmp;
	FILE *fptst;

	if ( ( fptst= fopen("./tstpwd.srt", "r") ) == NULL)
	{ perror("./tstpwd.srt: "); return 1; } 

	for ( nbusr= 0; nbusr < maxrnum ; nbusr++ ) {
		sortidx[nbusr] = nbusr;  
		if (fgets(sorttab[nbusr], (maxrlen+2), fptst) == NULL )
			break;
	}
	if ( fclose(fptst)!= 0)
   			{perror(" tstpwd.srt  "); return 5;}
   
	i = 0;
	for(i = 0; i < nbusr; i++){
		for(j = i+1; j < nbusr; j++){
			if(strcmp(sorttab[i],sorttab[j]) > 0){
				char *temp = (char *) malloc(sizeof(char) * (maxrlen+1));
				int p = 0;
				while(p < (maxrlen)){
					temp[p] = sorttab[i][p];
					p++;
				}
				strcpy(sorttab[i], sorttab[j]);
				strcpy(sorttab[j], temp);
			}
		}
	}

	for (i=0; i < nbusr; i++)
		{
			printf("acct[%d] = %s \n", i, sorttab[i]);
		
		}
	return 0;
}