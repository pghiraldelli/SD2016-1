#include "mysocket.h"

/* Structure of arguments to pass to client thread */
struct TArgs {
    TSocket cliSock;   /* socket descriptor for client */
};

TSocket srvSock, cliSock;        /* server and client sockets */
char *IP;
unsigned short porta;


/* Handle client request */
int HandleRequest(void *args) {
    char str[100];
    TSocket cliSock;

    /* Extract socket file descriptor from argument */
    cliSock = ((struct TArgs *) args) -> cliSock;
    free(args);  /* deallocate memory for argument */

    /* Receive the request */
    if (ReadLine(cliSock, str, 99) < 0){
        ExitWithError("ReadLine() failed");
    } else printf("%s",str);

    /*... do something ... */

    /* Send the response */
    if (WriteN(cliSock, "ack\n", 4) <= 0){
        ExitWithError("WriteN() failed");
    }

    close(cliSock);
    return 1;
}


void criaSocket(){
    porta = atoi(argv[2]);
    struct TArgs *args;              /* argument structure for thread */

    if (argc == 1) {
        ExitWithError("Usage: server <local port>");
    }

    /* Create a passive-mode listener endpoint */
    srvSock = CreateServer(porta);
}

void registrar(){
    IP = argv[1];


}

void esperaConexao(){
    for (;;) { /* run forever */
        cliSock = AcceptConnection(srvSock);

        /* Create a memory space for client argument */
        if ((args = (struct TArgs *) malloc(sizeof(struct TArgs))) == NULL)
        { ExitWithError("malloc() failed"); }
        args->cliSock = cliSock;

        /* Handle the client request */
        if (HandleRequest((void *) args) != 1)
        { ExitWithError("HandleRequest() failed"); }
    }
}

void escreveMSG() {
    char str[100];
    int n;
    for(;;) {
        /* Write msg */
        scanf("%99[^\n]%*c",str);
        n = strlen(str);
        str[n] = '\n';
        if (WriteN(sock, str, ++n) <= 0){
            ExitWithError("WriteN() failed");
        }
        if (strncmp(str, "quit", 4) == 0) break;

        /* Receive the response */
        if (ReadLine(sock, str, 99) < 0) {
             ExitWithError("ReadLine() failed");
        }
        else printf("%s",str);
    }
}

int main(int argc, char *argv[]) {
    criaSocket();
    registrar();
    esperaConexao();

}
