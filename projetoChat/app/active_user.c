#include "mysocket.h"

TSocket srvSock;
Usuario usuario;

void conecta(){
    srvSock = ConnectToServer(usuario.IP, usuario.porta);
}

void conversa() {
    char str[100];
    int n,ret;
    fd_set set;
    for(;;) {
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
            else{
                if (WriteN(srvSock, str, ++n) <= 0)
                  { ExitWithError("WriteN() failed"); }
                if (strncmp(str, "quit", 4) == 0) break;
            }
        }

        /* Read from srvSock */
        if (FD_ISSET(srvSock, &set)) {
            if (ReadLine(srvSock, str, 99) < 0)
            { ExitWithError("ReadLine() failed");}
            else{
                if (strncmp(str, "FIM", 3) == 0) {
                    close(srvSock);
                    break;
                }
                else{
                    printf("%s",str);
                }
            }
        }
    }
}


int main(int argc, char *argv[]) {
    usuario.IP = argv[1];
    usuario.porta = 2017;
    conecta();
    conversa();
    return 0;
}
