// The Application Main Server
// Servidor de Usuarios

#include "mysocket.h"  
#include <pthread.h>

#define BUFSIZE 100
#define NTHREADS 100
#define NUSUARIOS 100

/* Structure of arguments to pass to client thread */
struct TArgs {
  TSocket cliSock;   /* socket descriptor for client */
};

Usuario usuarios[NUSUARIOS];

void registrar(char *str)
{
  // IP 16
  // porta 4
  // nome 20
  // error se nao atender essas especificacoes
}

void listar()
{
  printf("Usuarios Ativos:\n");
  for (int i = 0; i < NUSUARIOS; ++i)
  {
    if (usuarios[i] != null)
      printf("%s\n", usuarios[i].nome); //CONCATENAR NUMA STRING TODAS AS INFORMACOES
  }
}

void removeUsuario(int cliSock)
{
  for (int i = 0; i < NUSUARIOS; ++i)
  {
    if (usuarios[i].usuarioID == cliSock)
    {
      usuarios[i] = null;
      break;
    }
  }
}

void finalizar(int cliSock)
{
  removeUsuario(cliSock);
  close(cliSock);
  pthread_exit(NULL);
}

void executa(int cliSock, char *str)
{
    char *ops = strtok(str, " ");

  if (strcmp(ops,"registrar")==0){
    registrar(str);
  }
  else if (strcmp(ops,"listar\n")==0){
    listar();
  }
  else if (strcmp(ops,"finalizar\n")==0){
    finalizar();
  }
  else{
    sprintf(str, "Comando nao encontrado!\n");
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
    if (ReadLine(cliSock, str, BUFSIZE-1) < 0) { 
      ExitWithError("ReadLine() failed"); 
    } else {
      executa(str);
    }
 
    /* Send the response */
    if (WriteN(cliSock, str, strlen(str)) <= 0)  
      { ExitWithError("WriteN() failed"); }  
  }
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
      }
    }
  }
  
  printf("Server will wait for the active threads and terminate!\n");
  /* Wait for all threads to terminate */
  for(i=0; i<tid; i++) {
    pthread_join(threads[i], NULL);
  }
  return 0;
}
