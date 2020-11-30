#include <stdio.h>

/* Programa chamado pelo exec */

void delay(void)
{
    for(int i = 0; i < 100; i++)
        for(int j = 0; j < 1000; j++);
}

int main()
{
    for(int i = 0; i < 10; i++)
    {
        printf("%d ", i);
        delay();
    }

    printf("\n");
    return 0;
}