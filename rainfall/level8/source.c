#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *auth = NULL;
char *service = NULL;

int main()
{
    char buffer[128];

    while (1)
    {
        printf("%p, %p\n", auth, service);

        fgets(buffer, 128, stdin);

        if (strncmp(buffer, "auth ", 5) == 0)
        {
            auth = malloc(4);     // allocate only 4 bytes
            *auth = 0;            // set first byte to 0

            if (strlen(buffer + 5) <= 30)
                strcpy(auth, buffer + 5);  // potential overflow
        }

        if (strncmp(buffer, "reset", 5) == 0)
        {
            free(auth);
        }

        if (strncmp(buffer, "service", 6) == 0)
        {
            service = strdup(buffer + 7);
        }

        if (strncmp(buffer, "login", 5) == 0)
        {
            if (auth != NULL && auth[32] != 0)
                system("/bin/sh");
            else
                fwrite("Password:\n", 1, 10, stdout);
        }
    }

    return 0;
}
