![Fork e Exec](https://www.cse.unsw.edu.au/~cs1521/18s2/lectures/week07/Pics/processes/fork-exec.png)

# _fork()_ e _exec()_ *System Call*
## Introdução
No artigo anterior descreveu a biblioteca hardware que necessária para viabilizar o desenvolvimento dos exemplos, com o desenvolvimento da camada abstração concluída e instalada. Mas para prossegir ainda falta um passo de extrema
importância que será abordado nesse artigo que são a combinação de _fork_ e _exec_.
## *fork()*
O _fork()_ é um *system call* capaz de criar um novo processo denominado filho, que é uma cópia exata do processo original denominado pai. Para utilizar o _fork()_ é necessário incluir os seguintes _includes_:
```c
# include <sys/types.h>
# include <unistd.h>

pid_t fork (void);
```

Para exemplificar vamos criar uma aplicação muito conhecido por todos, o famoso _hello world_, a seguir vemos o seguinte código fonte que tem a finalidade de mostrar no console a _string hello world_: 
```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    fork();
    printf("hello world.\n");
    return 0;
}
```

O _output_ desse código será:

```bash
hello world.
hello world.
```

O resultado do código anterior resultou em uma grande confusão agora é necessário explicar o que ocorre por de trás das cortinas.Pessoa : Duas _strings hello world_?
Alguém : Isso mesmo, uma mensagem do processo original e outro do processo clone.
Pessoa : Mas como é possível haver um clone?
Alguém : Não acredita? Tudo bem então vamos provar.
Pessoa : Mas como fazer isso?
Alguém : Bom para isso precisamos encontrar evidências sobre esse mistério.
Pessoa : Como pode apresentar duas mensagens sendo que o programa é um só? E somente existe uma ocorrência de _printf_. Bizarro.
Alguém : Não nos resta alternativa, precisamos interrogar, vamos perguntar para o programa quem é ele com a função _getpid_.

```c
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main(void)
{
    fork();
    printf("Quem eh voce? Eu sou o processo de pid %d.\n", getpid());
    return 0;
}
```

Ao executar é possível ver a saída com o resultado:

```bash
Quem eh voce? Eu sou o processo de pid 19489.
Quem eh voce? Eu sou o processo de pid 19490.
```

Oh não, isso deve ser um pesadelo é o ataque dos clones. Hey vai com calma não é isso que você está pensando, isso é só um Jutsu Clone das sombras, é uma técnica para poder dividir o trabalho. Bom isso seria útil para realizar as tarefas de casa. Bom a figura abaixo demonstra o que ocorre durante o _fork_:

![Processo sendo clonado](https://indradhanush.github.io/images/shell-part-2/execvp.jpg)

Como apresentado na figura é possível notar que o fluxo é ramificado após a execução do _fork_, para verificar quem é quem, com o retorno do _fork_ é possível distiguir qual processo é qual. Vejamos mais um exemplo para demonstrar o resultado do _fork_:

```c
#include <sys/types.h>
#include <unistd.h>
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
        printf("O que eu estou fazendo aqui, cara fui clonado.\n");
    }else
    {
        printf("Eu sou o verdadeiro.\n");
    }
    return 0;
}
```

A saída fica assim :

```bash
Eu sou o verdadeiro.
O que eu estou fazendo aqui, cara fui clonado
```

Como pode ser observado o clone retorna o valor 0, o original recebe o PID do clone, e se houver erro o retorno é -1.

Mas para que serve isso? Para o nosso propósito precisamos apresentar mais um componente.

## *exec()*
Clone : Cara não acredito?
Alguém : O que foi que aconteceu?
Clone: Eu não passo de um mero clone.
Alguém: Não cara, não diga isso você pode ser um clone mas você pode seguir o seu próprio caminho, ser o que quiser ser.
Clone: Sério?
Alguém: Sim você precisa somente de um contexto, e assim seguir o seu próprio caminho.
Clone: Nossa isso seria ótimo mas como eu faço isso?
Alguém: Temos um recurso bastante versátil conhecido como _exec_, com ele é possível carregar um outro programa, que trocará todo o contexto anterior, e você será promovido, dessa forma você pode ser que você quiser.

O _exec_ é um _system call_ capaz de carregar outro programa, trocando todo o contexto do programa que o invoca. O _exec_ possui várias formas de ser utilizado e suas variações se devem ao passar do tempo programadores o adaptavam conforme suas necessidades resultando nessa quantidade de funções, para melhor atendê-lo verifique o _man pages_ para maiores informações sobre o _exec_:


<ul>
    <li>int execl(const char *path, const char *arg, .../* (char  *) NULL */);</li>
    <li>int execlp(const char *file, const char *arg, ... /* (char  *) NULL */);</li>
    <li>int execle(const char *path, const char *arg, ... /*, (char *) NULL, char * const envp[]*/);</li>
    <li>int execv(const char *path, char *const argv[]);</li>
    <li>int execvp(const char *file, char *const argv[]);</li>
    <li>int execvpe(const char *file, char *const argv[], char *const envp[]);</li>
</ul>

Para exemplicar vamos criar uma outra aplicação

O programa a seguir é o responsável pela criação do clone. após clonado o programa clone decide que quer se tornar um contador e seguir carreira nessa área.

```c
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
```

O programa a seguir apresenta em um novo momento, talvez um momento de realização para o clone, que mesmo sendo um clone pode decidir qual caminho trilhar, e decidiu virar um contador, muito bom meu caro amigo persiga o seu sonho.

```c
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
```

## Conclusão
Neste artigo foi apresentado como se utiliza o _fork_ atráves de alguns exemplos simples, e como utilizar o clone para invocar um outra aplicação utilizando o comando _exec_. No próximo artigo iremos abordar o primeiro IPC, o *shared file*. E finalizamos mais um artigo com um final feliz e que nosso clone possa realizar outras atividades se tornando o que ele quiser.


