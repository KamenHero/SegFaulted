#include <stdio.h>
#include <stdlib.h>

void v() {
    char buffer[520];

    fgets(buffer, 512, stdin);
    printf(buffer);

    if (m == 64) {
        printf("Wait what?!\n");
        system("/bin/sh");
    }
}

int main() {
    v();
    return 0;
}
