#include <stdio.h>
#include <stdlib.h>

char *arr[1] = {}; 
size_t n = 100;

int main(void)
{
    size_t cur = 0; 
    while (1)
    {
        char *s;
        getline(&s, &cur, stdin);

        printf("Received size %i: %s", (int)cur, s);

        if (cur <= 0)
            break;
    }
}
