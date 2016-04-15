// A multithread echo server 

#include "mysocket.h"  
#include <pthread.h>

#define BUFSIZE 100
#define NTHREADS 100

/* Structure of arguments to pass to client thread */
struct TArgs {
  TSocket cliSock;   /* socket descriptor for client */
};

int numero_clientes = 0;

void calcula(char *str){
  int num1, num2;
  char *operador;
  char *token = strtok(NULL, " ");
  
  num1 = atoi(token);

  token = strtok(NULL, " ");
  operador = token;

  token = strtok(NULL, " ");
  num2 = atoi(token);

  if(strcmp(operador, "+") == 0){
    sprintf(str, "%d\n", num1 + num2);
  }
  else if(strcmp(operador, "-") == 0){
    sprintf(str, "%d\n", num1 - num2);
  }
  else{
    sprintf(str, "Operacao mal formada!\n");
  }
}

void verificaUsuariosOnline(char *str){
    sprintf(str, "Numero de clientes online: %d\n", numero_clientes);
}

void imprime(char *str){
    char *token = strtok(NULL, "\n");
    sprintf(str, "%s\n", token);
}


void executa(char *str){
  
  char *ops = strtok(str, " ");

  if (strcmp(ops,"calcula")==0){
    calcula(str);
  }
  else if (strcmp(ops,"imprime")==0){
    imprime(str);
  }
  else if (strcmp(ops,"online\n")==0){
    verificaUsuariosOnline(str);
  }
  else{
    sprintf(str, "Comando nao encontrado\n");
  }
}

/* Handle client request */
void * HandleRequest(void *args) {
  char str[BUFSIZE];
  TSocket cliSock;

  /* Extract socket file descriptor from argument */
  cliSock = ((struct TArgs *) args) -> cliSock;
  free(args);  /* deallocate memory for argument */

  for(;;) {
    /* Receive the request */
    if (ReadLine(cliSock, str, BUFSIZE-1) < 0) 
      { ExitWithError("ReadLine() failed"); 
    } 
    else {
        executa(str);
    }  
    if (strncmp(str, "quit", 4) == 0) break; 
 
    /* Send the response */
    if (WriteN(cliSock, str, strlen(str)) <= 0)  
      { ExitWithError("WriteN() failed"); }  
  }
  close(cliSock);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  TSocket srvSock, cliSock;        /* server and client sockets */
  struct TArgs *args;              /* argument structure for thread */
  pthread_t threads[NTHREADS];
  int tid = 0;
  fd_set set;  /* file description set */
  int ret, i;
  char str[BUFSIZE];

  if (argc == 1) { ExitWithError("Usage: server <local port>"); }

  /* Create a passive-mode listener endpoint */  
  srvSock = CreateServer(atoi(argv[1]));

  printf("Server read!\n");
  /* Run forever */
  for (;;) { 
    /* Initialize the file descriptor set */
    FD_ZERO(&set);
    /* Include stdin into the file descriptor set */
    FD_SET(STDIN_FILENO, &set);
    /* Include srvSock into the file descriptor set */
    FD_SET(srvSock, &set);

    /* Select returns 1 if input available, -1 if error */
    ret = select (FD_SETSIZE, &set, NULL, NULL, NULL);
    if (ret<0) {
       WriteError("select() failed"); 
       break;
    }

    /* Read from stdin */
    if (FD_ISSET(STDIN_FILENO, &set)) {
      scanf("%99[^\n]%*c", str);
      if (strncmp(str, "FIM", 3) == 0) {
        close(srvSock);
        break;
      }
    }

    /* Read from srvSock */
    if (FD_ISSET(srvSock, &set)) {
      if (tid == NTHREADS) { 
        WriteError("number of threads is over"); 
        break; 
      }
      
      /* Spawn off separate thread for each client */
      cliSock = AcceptConnection(srvSock);

      /* Create separate memory for client argument */
      if ((args = (struct TArgs *) malloc(sizeof(struct TArgs))) == NULL) { 
        WriteError("malloc() failed"); 
        break;
      }
      args->cliSock = cliSock;

      /* Create a new thread to handle the client requests */
      if (pthread_create(&threads[tid++], NULL, HandleRequest, (void *) args)) { 
        WriteError("pthread_create() failed"); 
        break;
      }
      numero_clientes++;
    }
  }
  
  printf("Server will wait for the active threads and terminate!\n");
  /* Wait for all threads to terminate */
  for(i=0; i<tid; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
