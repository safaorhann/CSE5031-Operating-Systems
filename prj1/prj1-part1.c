#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/utsname.h>

int main(int argc, char **argv)
{
char *value;
int i=1;
printf("running <%s> with %d parameter \n", *argv, argc-1);
struct utsname buffer;
uname(&buffer);


while (i < argc)
{
    value = getenv(argv[i]);
    printf("%s: %s \n",argv[i],value);
    i++;
}

value = getenv("HOME");
printf("HOME = %s \n",value);
value = getenv("USER");
printf("USER = %s \n",value);
value = getenv("PATH");
printf("PATH = %s \n",value);

printf("system name = %s\n",buffer.sysname);
printf("node name = %s\n", buffer.nodename);
printf("release = %s\n", buffer.release);
printf("version = %s\n", buffer.version);
printf("machine = %s\n", buffer.machine);

#ifdef _GNU_SOURCE
 printf("domain name = %s\n", buffer.domainname);
#endif

return 0;

}