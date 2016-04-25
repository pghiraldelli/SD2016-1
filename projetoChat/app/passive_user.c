#include "mysocket.h"

TSocket srvSock, cliSock;
char *IP;
unsigned short porta;

void criaSocket(){
    porta = 2017;
    srvSock = CreateServer(porta);
}

void registrar(){
    porta = 2016;
    TSocket servSock;
    servSock = ConnectToServer(IP,porta);
    printf("Entre com um nome de usuario: \n");
    char nome[20];
    char str[22];
    scanf("%s",nome);
    sprintf(str, "1 %s\n", nome);
    int n = strlen(str);
    str[n] = '\n';
    if (WriteN(servSock, str, ++n) <= 0)
    { ExitWithError("WriteN() failed"); }

    // DEPOIS FAZER UM SELECT PRA SE O CARA QUISER TERMINAR ANTES DE COMECAR
}

void conversa(TSocket cliSock) {
    char str[100];
    int n,ret;
    fd_set set;
    for(;;) {
        /* Initialize the file descriptor set */
        FD_ZERO(&set);
        /* Include stdin into the file descriptor set */
        FD_SET(STDIN_FILENO, &set);
        /* Include srvSock into the file descriptor set */
        FD_SET(cliSock, &set);

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
                close(cliSock);
                break;
            }
            else{
                if (WriteN(cliSock, str, ++n) <= 0)
                  { ExitWithError("WriteN() failed"); }
                if (strncmp(str, "quit", 4) == 0) break;
            }
        }

        /* Read from srvSock */
        if (FD_ISSET(cliSock, &set)) {
            if (ReadLine(cliSock, str, 99) < 0)
            { ExitWithError("ReadLine() failed");}
            else{
                if (strncmp(str, "FIM", 3) == 0) {
                    close(cliSock);
                    break;
                }
                else{
                    printf("%s",str);
                }
            }
        }
    }
}

void esperaConexao(){
    for (;;) {
        cliSock = AcceptConnection(srvSock);
        conversa(cliSock);
    }
}

int main(int argc, char *argv[]) {
    IP = argv[1];
    criaSocket();
    registrar();
    esperaConexao();
    return 0;
}
