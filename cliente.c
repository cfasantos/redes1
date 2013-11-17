#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>


#define SERVER_PORT 5000    // Porta do server fixada
#define SERVER_IP "127.0.0.1"    // IP do server fixado
#define MAXBUFLEN 100

#define MY_PORT 7000    // Porta do cliente fixada

int main(int argc, char *argv[])
{
    int sockfd;
    struct sockaddr_in their_addr; // endereço do servidor
    //struct sockaddr_in my_addr;    // meu endereço
    char buf[MAXBUFLEN];
    //size_t addr_len;
    //cria o socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
        perror("cliente: socket()");
    }

    //preenchendo estrutura de endereçamento
    their_addr.sin_family = AF_INET;         
    their_addr.sin_port = htons(SERVER_PORT);	// porta do servidor
    their_addr.sin_addr.s_addr = inet_addr(SERVER_IP);	// IP do servidor
    memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);
    int a = 0;
    int numbytes;
    
    char mensagem[MAXBUFLEN];
    while(a == 0){
        printf("entre com a mensagem:\n");
        scanf("%[^\n]s",mensagem);
        getchar();
        printf("Mensagem enviada para o servidor com conteúdo  \"%s\"\n", mensagem);
        // envia a mensagem para o servidor
        if ((numbytes = sendto(sockfd, mensagem, strlen(mensagem), 0, (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
            perror("Erro ao enviar mensagem ao servidor");
            exit(1);
        }
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
                                 NULL, NULL)) == -1) {
            perror("Erro ao receber mensagem do servidor");
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("Recebido pacote do servidor com conteúdo \"%s\"\n", buf);
    }
    
    close(sockfd);

    return 0;
}
