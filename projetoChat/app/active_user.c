#include "mysocket.h"

TSocket sock;

void conecta(Usuario usuario){
    if (argc != 3) {
        ExitWithError("Usage: client <remote server IP> <remote server Port>");
    }

    usuario.IP = argv[1];
    usuario.porta = atoi(argv[2]);

    /* Create a connection */
    sock = ConnectToServer(usuario.IP, usuario.porta);
}

void desconecta(){
    close(sock);
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
    Usuario usuario;
    conecta(usuario);
    escreveMSG();

    return 0;
}
