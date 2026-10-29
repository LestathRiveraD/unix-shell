#define _POSIX_C_SOURCE 200809L // strdup is a POSIX function, apparently my operating system was not exposing  strdup, so C was assuming its return value was an integer. Adding this line fixed the issue
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **arr;

int main(void)
{
    size_t size = 1, next = 0;
    arr = malloc(sizeof(char*) * size);
    
    if (arr == NULL)
        return 1;

    char* s = NULL; 
    size_t n;
    while (getline(&s, &n, stdin) != -1)
    {
        if (next >= size)
        {
            size *= 2;
            arr = realloc(arr, sizeof(char**) * size + 1); 
        }
        arr[next] = strdup(s); 
        next++;
    }
    free(s);
    printf("%s\n", s);
    
    for (int i = (int)next - 1; i >= 0; i--) 
    {
        printf("%i: %s", i + 1, arr[i]);
        free(arr[i]);
    }

    free(arr);

    return 0;
}
