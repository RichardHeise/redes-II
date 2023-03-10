/* Autores:
        Pietro Polinari Cavassin GRR20190430
        Richard Fernando Heise Ferreira GRR20191053
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define ERROR_CLINE -1
#define ERROR_GETIP -2
#define ERROR_OPENSCKT -3
#define ERROR_SENDATA -4

int main(int argc, char *argv[]) { 

    int socket_description;
    struct sockaddr_in sa;
    struct hostent *hp;
    char *host;

    if(argc != 4) {
      puts("Correct usage: <client> <server-name> <port> <data>");
      exit(ERROR_CLINE);
    }

    host = argv[1];
    int num_message = atoi(argv[3]);

    /* Procura a struct hostent correspondente ao host do argumento */
    if( (hp = gethostbyname(host)) == NULL ) {
      puts("Nao consegui obter endereco IP do servidor.");
      exit(ERROR_GETIP);
    }

    /* Configura porta, endereço e tipo de endereço do host no socket. */
    bcopy( (char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length );
    sa.sin_family = hp->h_addrtype;
    sa.sin_port = htons(atoi(argv[2]));

    /* Cria socket */
    if( (socket_description=socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0 ) {
      puts("Couldn't open socket.");
      exit(ERROR_OPENSCKT);
    }

    /* Loop de envio de mensagens */
    char data[10] = {0};
    for (int i = 1; i <= num_message; i++) {
        
        /* Copia número de mensagem atual para string data e tenta enviar no socket. */
        sprintf(data, "%d ", i);
        if( sendto(socket_description, data, strlen(data)+1, 0, (struct
            sockaddr *) &sa, sizeof sa) != strlen(data)+1) {
            puts("Couldn't send data."); 
            exit(ERROR_SENDATA);
        }

    }

    close(socket_description);
    return 0;
}
