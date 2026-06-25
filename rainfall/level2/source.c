#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* p()
{
    // 64-byte stack buffer
    char buffer[64];

    // Will store the return address
    void* return_addr;

    fflush(stdout);

    gets(buffer);

    // GCC/Clang builtin: get current function's return address
    return_addr = __builtin_return_address(0);

    // Check if return address is in stack region (0xbxxxxxxx) (in other words, if the return address points to the stack, abort.)
    if (((unsigned int)return_addr & 0xb0000000) == 0xb0000000)
    {
        printf("%p\n", return_addr);
        exit(1);
    }

    // Print user input
    puts(buffer);

    // Duplicate buffer into heap memory
    return strdup(buffer);
}

int main()
{
    p();
    return 0;
}