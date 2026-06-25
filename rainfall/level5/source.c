
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void o()
{
    system("/bin/sh");
    exit(1);
}

void n()
{
    char buffer[1024];

    fgets(buffer, 1024, stdin);
    printf(buffer);   // VULNERABILITY
    
    exit(1);
}

int main() 
{
    n();
    return (0);
}
