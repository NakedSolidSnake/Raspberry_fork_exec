<p align="center">
    <img src="https://www.cse.unsw.edu.au/~cs1521/18s2/lectures/week07/Pics/processes/fork-exec.png" />
</p>

# _fork()_, _exec()_ e _daemon()_ *System Call*

## Tópicos
* [Introdução](#introdução)
* [fork](#fork)
* [exec](#exec)
* [daemon](#daemon)
* [Conclusão](#conclusão)
* [Referências](#referências)


## Introdução
No artigo anterior descreveu a biblioteca hardware que necessária para viabilizar o desenvolvimento dos exemplos, com o desenvolvimento da camada abstração concluída e instalada. Mas para prossegir ainda falta um passo de extrema importância que será abordado nesse artigo que são a combinação de _fork_ e _exec_, e a definição do que é um _daemon_.
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

<p align="center">
    <img src="https://indradhanush.github.io/images/shell-part-2/execvp.jpg"/>
</p>

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


```c
int execl(const char *path, const char *arg, .../* (char  *) NULL */);
```
```c
int execlp(const char *file, const char *arg, ... /* (char  *) NULL */);
```
```c
int execle(const char *path, const char *arg, ... /*, (char *) NULL, char * const envp[]*/);
```
```c
int execv(const char *path, char *const argv[]);
```
```c
int execvp(const char *file, char *const argv[]);
```
```c
int execvpe(const char *file, char *const argv[], char *const envp[]);
```

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

## *daemon()*
Basicamente _daemon_ é uma instância que roda em segundo plano, sem a interação de _STDIN_, _STDOUT_ e _STDERR_. Normalmente fornece algum tipo de serviço como por exemplo o sshd(security shell daemon) que ouve conexões usando protocolo ssh e atua como servidor para o protocolo, possui um ciclo de vida desde o _power on_ da máquina até o _shutdown_(caso não haja _segfault_ ;P). No tópico de _fork_, é gerado uma cópia do programa corrente, esse programa filho é uma espécie de daemon, pois roda em segundo plano após o _fork_. Para a criação de um _daemon_ existe um roteiro onde alguns passos devem ser seguidos, e existe também uma _system call_ que abstrai todo o processo de criação:

### Criação pelo modelo tradicional
* Gerar o clone do processo através do _fork_ para que o processo rode em background
```c
process_id = fork();
if(process_id < 0)
{
printf("fork falhou.\n");
exit(1);
}

if(process_id > 0)
{
printf("PID do processo filho %d\n", process_id);
exit(0);
}
```
* Alterar as permissões de acesso, para esse caso o argumento 0 significa que as permissões serão herdadas
```c
umask(0);
```
* Criar uma nova sessão 
```c
sid = setsid();
if(sid < 0)
{
exit(1);
}
```
* Configurar o diretório de trabalho
```c
chdir("/");
```

* Fechar a entrada padrão, a saída padrão e a saída de erro padrão
```c
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```
* Executar o serviço proposto, que vai logar a contagem a cada um segundo
```c
int i = 0;

  while(1)
  {
    call_log(i++);
    sleep(1);
  }
```

Aqui está a descrição completa do exemplo
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <syslog.h>

void call_log(int counter)
{
    openlog("Daemon", LOG_PID | LOG_CONS , LOG_USER);
    syslog(LOG_INFO, "Counter : %d", counter);
    closelog();
}

int main(int argc, char* argv[])
{
  pid_t process_id = 0;
  pid_t sid = 0;

  process_id = fork();
  if(process_id < 0)
  {
    printf("fork falhou.\n");
    exit(1);
  }

  if(process_id > 0)
  {
    printf("PID do processo filho %d\n", process_id);
    exit(0);
  }

  umask(0);

  sid = setsid();
  if(sid < 0)
  {
    exit(1);
  }

  chdir("/");

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);


  int i = 0;

  while(1)
  {
    call_log(i++);
    sleep(1);
  }
  
  return 0;
}
```

### Criação pela _system call_

A _system call daemon_ capaz de abstrair tudo isso em uma única chamada
```c
#include <unistd.h>

int daemon(int nochdir, int noclose);
```

O mesmo exemplo usando _daemon_ _system call_
```c
#include <stdio.h>
#include <syslog.h>
#include <unistd.h>
#include <stdbool.h>

void call_log(int counter)
{
    openlog("Daemon", LOG_PID | LOG_CONS , LOG_USER);
    syslog(LOG_INFO, "Counter : %d", counter);
    closelog();
}

int main(int argc, char *argv[])
{
    int i = 0;

    daemon(0, 0);    
    
    while(true)
    {
        call_log(i++);
        sleep(1);
    }
    return 0;
}
```

## Conclusão
Neste artigo foi apresentado como se utiliza o _fork_ atráves de alguns exemplos simples, e como utilizar o clone para invocar um outra aplicação utilizando o comando _exec_, dessa forma podemos criar serviços para rodem .

## Referências
* [Linux Programming Interface](https://www.amazon.com.br/dp/B004OEJMZM/ref=dp-kindle-redirect?_encoding=UTF8&btkr=1)
* [daemon](https://www.go4expert.com/articles/writing-linux-daemon-process-c-t27616/)
