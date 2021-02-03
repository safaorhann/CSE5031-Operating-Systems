#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{

	struct passwd *pwent;

	while((pwent = getpwent()) != NULL)
	{
		if(strcmp(pwent->pw_name, argv[1]) == 0)
		{
			printf("User Name: %s\n", pwent->pw_name);
			printf("UID: %d\n", pwent->pw_uid);
			printf("GID: %d\n", pwent->pw_gid);
			printf("Home Directory: %s\n", pwent->pw_dir);
			break;
		}
	}

	return 0;
}