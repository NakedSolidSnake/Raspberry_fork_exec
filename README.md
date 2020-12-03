![Fork e Exec](https://www.cse.unsw.edu.au/~cs1521/18s2/lectures/week07/Pics/processes/fork-exec.png)

# _fork()_ e _exec()_ *System Call*
## Introdução
No artigo anterior descreveu a biblioteca hardware que necessária para viabilizar o desenvolvimento dos exemplos, com o desenvolvimento da camada abstração concluída e instalada. Mas para prossegir ainda falta um passo de extrema
importância que será abordado nesse artigo que são a combinação de _fork_ e _exec_.
## *fork()*
O _fork()_ é um *system call* capaz de criar um novo processo denominado filho, que é uma cópia exata do processo original denominado pai. Para utilizar o _fork()_ é necessário incluir os seguintes includes:
```c
# include <sys/types.h>
# include <unistd.h>

pid_t fork ( void );
```
## *exec()*
## Conclusão
Neste artigo foi apresentado como se utiliza o _fork_ atráves de alguns exemplos simples, e como utilizar o clone para invocar um outra aplicação utilizando o comando _exec_. No próximo artigo iremos abordar o primeiro IPC, o *shared file*. E finalizamos mais um artigo com um final feliz e que nosso clone possa realizar outras atividades se tornando o que ele quiser.


