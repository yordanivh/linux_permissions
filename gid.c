#include <stdio.h>
#include <unistd.h>
int main () {
int real = getgid();
int egid = getegid();
printf("The REAL GID =: %d\n", real);
printf("The EFFECTIVE GID =: %d\n", egid);
}
