#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define MYPORT 5000    // porta usada para receber conexao
#define SERVER_IP "127.0.0.1"    // IP do server fixado
#define MAXBUFLEN 100
#define MINSIZEMSG 10
#define REGISTER_202 "202 Accepted 127.0.0.1 5000"
#define REGISTER_402 "402 Duplicated Name"
#define UNREGISTER_200 "200 Ok"
#define UNREGISTER_404 "404 Not Found"
#define INFO_200 "200 Ok "
#define REGISTER "REGISTER"
#define UNREGISTER "UNREGISTER"
#define INFO "INFO"
#define NOT_IMPLEMENTED "501 Not Implemented"

int main(void)
{
    int sockfd;
    struct sockaddr_in my_addr;    // meu endereço 
    struct sockaddr_in their_addr; // endereço do cliente
    int numbytes;
    char buf[MAXBUFLEN];
    size_t addr_len;

    // cria socket
    if ((sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP)) == -1) {
    	perror("servidor: socket()");
    }
    
    //preenchendo estruturas de endereçamento
    my_addr.sin_family = AF_INET;       // familia de enderacamento
    my_addr.sin_port = htons(MYPORT);	// porta do servidor
    my_addr.sin_addr.s_addr = INADDR_ANY;	// aceitando qualquer IP
    memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);   // funcao utilizada para completar parte da estrutura que nao eh utilizada
	
	
    // associa o socket a porta definida
    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof my_addr) == -1) {
        close(sockfd);
        perror("Erro ao realizar o bind do servidor");
    }
    
    int a = 1;
    
    char retMsg[MAXBUFLEN];
    while(a){
        strcpy(retMsg, "");
        printf("Aguardando requisições do cliente...\n");

        addr_len = sizeof their_addr;

        // a funcao recvfrom eh chamada e aguarda pela mensagem
        if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,(struct sockaddr *)&
                                 their_addr, &addr_len)) == -1) {
            perror("Erro no recebimento da mensagem");
            exit(1);
        }

        buf[numbytes] = '\0';
        printf("Recebido pacote com conteúdo \"%s\"\n", buf);
        
        char clntMsg[MAXBUFLEN];
        sscanf(buf,"%s",clntMsg);
        
        if(strcmp(clntMsg, REGISTER)==0){
            strcat(retMsg,REGISTER_202);
        }else{
            if(strcmp(clntMsg, UNREGISTER) == 0){
                strcat(retMsg,UNREGISTER_200);
            }else{
                if(strcmp(clntMsg, INFO) == 0){
                    strcat(retMsg,INFO_200);
                    strcat(retMsg,inet_ntoa(their_addr.sin_addr));
                }else{
                    strcat(retMsg,NOT_IMPLEMENTED);
                }
            }
        }
        printf("Enviado pacote para o cliente %s %d com conteúdo \"%s\"\n",inet_ntoa(their_addr.sin_addr),their_addr.sin_port, retMsg);
        if ((numbytes = sendto(sockfd, retMsg, strlen(retMsg), 0, (struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
            perror("erro ao responder ao cliente");
            exit(1);
        }
        
    }
    close(sockfd);

    return 0;
}
