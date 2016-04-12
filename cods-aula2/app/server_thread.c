// A multithread echo server 

#include "mysocket.h"  
#include <pthread.h>

#define NTHREADS 100

/* Structure of arguments to pass to client thread */
struct TArgs {
  TSocket cliSock;   /* socket descriptor for client */
};

void calcula(char *str){
	int num1, num2;
	char *operador;
	char *vetor = strtok(NULL, " ");
	
	num1 = atoi(vetor);

	vetor = strtok(NULL, " ");
	operador = vetor;

	vetor = strtok(NULL, " ");
	num2 = atoi(vetor);

	if(strcmp(operador, "+") == 0){
		sprintf(str, "%d\n", num1 + num2);
	}
	else if(strcmp(operador, "-") == 0){
		sprintf(str, "%d\n", num1 - num2);
	}
}

void executa(char *str){
	
	char *ops = strtok(str, " ");

	printf("%s\n", str);

	if (strcmp(ops,"calcula")==0){
		calcula(str);
	}
	else if (strcmp(ops,"imprime")==0){
		ops = strtok(NULL, "\n");
		sprintf(str, "%s\n", ops);
	}
	else if (strcmp(ops,"online")==0){
		//online(str);
	}
	else{
		sprintf(str, "Comando nao encontrado\n");
	}
}

/* Handle client request */
void * HandleRequest(void *args) {
  char str[100];
  TSocket cliSock;

  /* Extract socket file descriptor from argument */
  cliSock = ((struct TArgs *) args) -> cliSock;
  free(args);  /* deallocate memory for argument */

  for(;;) {
    /* Receive the request */
    if (ReadLine(cliSock, str, 99) < 0) 
      { ExitWithError("ReadLine() failed"); 
    } else {
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

  if (argc == 1) { ExitWithError("Usage: server <local port>"); }

  /* Create a passive-mode listener endpoint */  
  srvSock = CreateServer(atoi(argv[1]));

  /* Run forever */
  for (;;) { 
    if (tid == NTHREADS) 
      { ExitWithError("number of threads is over"); }

    /* Spawn off separate thread for each client */
    cliSock = AcceptConnection(srvSock);

    /* Create separate memory for client argument */
    if ((args = (struct TArgs *) malloc(sizeof(struct TArgs))) == NULL)
      { ExitWithError("malloc() failed"); }
    args->cliSock = cliSock;

    /* Create a new thread to handle the client requests */
    if (pthread_create(&threads[tid++], NULL, HandleRequest, (void *) args)) {
      { ExitWithError("pthread_create() failed"); }
    }
    /* NOT REACHED */
  }
}
