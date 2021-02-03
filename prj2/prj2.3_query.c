#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>


typedef struct { // idx file record structure
	char actname [32+1];   // account name string +'\0'
	long int pwdoffset;    // passwd file offset
	int  rel;              // record length in ./passwd
} IdxRec, *pIdxRec;

char* foo(char * c, int idx, int idx2){
	printf("enter the comment\n");
	char *ch = (char*)malloc(sizeof(char)*(idx2 - idx));
	scanf("%s",ch);
	assert(strlen(ch) == idx2 - idx);
	char *s = (char*) malloc(sizeof(char)*strlen(c));
	char *c1 = (char*) malloc(sizeof(char)*(idx));
	strncpy(c1,c,idx);
	strcat(s,c1);strcat(s,ch);
	char c2[(strlen(c)-idx2)];
	int i, k = 0;for(i = idx2; i < strlen(c); i++){c2[k++] = c[i];}
	strcat(s,c2);
	return s;
}

int func(IdxRec* IdxTab, char* user , int len){
	int a=0, found = 0;
	while(a < len){
		if(strcmp(user,IdxTab[a].actname)==0)
			{
				found = 1;
				break;
			}
		a++;
	}
	if(found == 0) return -1;
	return a;
}




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
	printf("enter the account name\n");
	if (scanf("%s", user) == EOF) { perror("Invalid # user");return 1; }
	
	if (pread(fd, (void *) IdxTab, (sizeof(IdxRec) * buffer.st_size), 0) == -1)
	{
		perror("./idx write error: "); return 6;
	}
	close(fd);

	int cont = func(IdxTab, user, buffer.st_size / sizeof(IdxRec));
	int offset_;
	int size;
	char *c;
	int k,i,count;
	if(cont == -1){
		puts("not found error");return 5;
	}
	else{
		int flag = 0;
		fd = open("passwd", O_RDONLY);
		while(flag != 1 && cont < buffer.st_size / sizeof(IdxRec)){
			offset_ = IdxTab[cont].pwdoffset;
			
			size = IdxTab[cont].rel;
			
			c = (char*) malloc(sizeof(char)*size);
			pread(fd, (void*)c, size, offset_);
			
			k = 0, i = 0, count = 0;
			for(i = 0; i < strlen(c); i++){if(k == 4){break;}if(c[i] == ':'){k++;}}
			k = i;
			for(; i < strlen(c); i++){if(c[i] == ':'){break;}else{count++;}}
			if(count > 0)
				flag = 1;
			else
				cont++;
		}
		if(flag){
			close(fd);
			fd = open("passwd", O_WRONLY);
			c = foo(c, k, k+count);
			pwrite(fd, (void*)c, size,offset_);	
			close(fd);
		}
	}

	return 0;
}