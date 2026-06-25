#include <stdio.h>
#include <stdlib.h>

int run(void)
{
    // Print a message to stdout
    fwrite("Good... Wait what?\n", 19, 1, stdout);

    // Execute a shell
    return system("/bin/sh");
}

int main(void)
{
    // 64-byte stack buffer
    char buffer[64];

    // Vulnerable function: because it doesn't check the boundaries of the stack buffer so it allows the stack buffer overflow
    // then overflow can overwrite the return address of main() then the it will return an existing function (the run() function that will redirect to the system call)
    gets(buffer);

    return 0;
}