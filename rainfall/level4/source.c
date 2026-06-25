#include <stdio.h>
#include <stdlib.h>

int m = 0;

int p(char *buffer)
{
    printf(buffer);
    return(0);
}

void v()
{
    char buffer[1024];
    
    fgets(buffer, 1024, stdin);

    p(buffer);
    
    if (m == 16930116)
    {
        system("/bin/cat /home/user/level5/.pass");
	    return (0);
    }
}

int main() {
    v();
    return 0;
}