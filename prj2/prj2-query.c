#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void foo(char *c){
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

typedef struct { // idx file record structure
	char actname [32+1];   // account name string +'\0'
	long int pwdoffset;    // passwd file offset
	int  rel;              // record length in ./passwd
} IdxRec, *pIdxRec;

int main(int argc, char const *argv[])
{
	char *user = (char*) malloc(sizeof(char)*32);
	struct stat buffer;
	int status;
	status = stat("passwd.idx", &buffer);
	pIdxRec IdxTab;
	if((IdxTab = malloc ( sizeof(IdxRec) * buffer.st_size)) == NULL)
	{ 
		puts("buffer allocation error");return 3; 
	}	
	int fd;
	char *filename = "passwd.idx";
	fd = open(filename, O_RDONLY);
	if (scanf("%s", user) == EOF) { perror("Invalid # user");return 1; }
	
	if (read(fd, (void *) IdxTab, (sizeof(IdxRec) * buffer.st_size)) == -1)
	{
		perror("./idx write error: "); return 6;
	}
	close(fd);
	int i=0, found = 0;
	while(i < buffer.st_size / sizeof(IdxRec)){
		if(strcmp(user,IdxTab[i].actname)==0)
			{
				found = 1;
				break;
			}
		i++;
	}
	if(found){
		int offset_ = IdxTab[i].pwdoffset;
		int size = IdxTab[i].rel;
		fd = open("passwd", O_RDONLY);
		lseek(fd, offset_, SEEK_SET);
		char *c = (char*) malloc(sizeof(char)*size);
		read(fd, (void*)c, size);
		close(fd);
		//printf("%s\n", c);
		foo(c);
	}
	else{
		printf("Could not found\n");
	}
	

	return 0;
}