#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    pid_t is_clone = fork();
    
    if(is_clone == -1)
    {
        printf("Não foi possivel gerar um clone.\n");
    }
    else if( is_clone == 0)
    {
        printf("Eu sou o clone mas posso ser o que eu quiser.\n");
        printf("Vou virar um contador.\n");
        char *args[]={"./counter",NULL}; 
        execvp(args[0],args); 
        /* se der certo o programa não retorna */
    }else
    {
        printf("Eu sou o verdadeiro.\n");
    }
    return 0;
}