#include <stdio.h>
#include <grp.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{

	struct group *gr;
	int count = 0;
	
	while((gr = getgrent()) != NULL)
	{
		if(gr->gr_mem != NULL)
		{
			char **c = gr->gr_mem;
			int i = 0;

			while(c[i] != '\0')
			{
				if(strcmp(c[i], argv[1]) == 0)
				{
					printf("GID: %d\n", gr->gr_gid);
					printf("Group Name: %s\n\n", gr->gr_name);
					count = 1;
				}
				i++;
			}
		}
	}
	
	if(count == 0)
		printf("%s is not a member of any group!\n", argv[1]);

	return 0;
}