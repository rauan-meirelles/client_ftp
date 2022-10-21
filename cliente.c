#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


#define MAXIMOMSG 500

/**
 * programa cliente
 */
int main(int argc, char *argv[])
{
    struct hostent *host_info;
    struct in_addr *address;

    if (argc < 2)
    {
        printf("Uso: %s <hostname>\n",argv[0]);
        exit(1);
    }

    // obtendo o IP do host passado, consulta ao servidor DNS
    host_info = gethostbyname(argv[1]);

    if (host_info == NULL)
    {
        printf("Nao pude encontrar %s\n",argv[1]);
    }
    else
    {
        address = (struct in_addr *) (host_info->h_addr);
    }
    
    // numero de filosofos
    char buffer[MAXIMOMSG + 1]; // para incluir o terminador nulo
    int tamanho, meusocket;
    struct sockaddr_in destinatario;
    char msg[100];

    meusocket = socket(AF_INET, SOCK_STREAM, 0);

    destinatario.sin_family = AF_INET;

    // ip do servidor - 127.0.0.1 se estiver rodando na sua mesma maquina
    //destinatario.sin_addr.s_addr = inet_addr("195.144.107.198");
    destinatario.sin_addr.s_addr = inet_addr(inet_ntoa(*address));

    // porta do servidor
    destinatario.sin_port = htons(21);

    // zerando o resto da estrutura
    memset(&(destinatario.sin_zero), '\0', sizeof(destinatario.sin_zero));

    connect(meusocket, (struct sockaddr *)&destinatario, sizeof(struct sockaddr));

    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    printf("Conectado ao servidor %s (%s) na porta 21\n", argv[1], inet_ntoa(*address));
    char username[100];
    char username_code[100] = "USER ";
    char sub[4];
    int status = 530;
    char password[100];
    char password_code[100] = "PASS ";
    
    strcpy(username, "");
    printf("Informe o Login:");
    scanf("%s", username);
    getchar();
    strcat(username_code, username);
    strcat(username_code, "\r\n");
    strcpy(msg, username_code);
    send(meusocket, msg, strlen(msg), 0);

    strcpy(buffer, "");
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    printf("%s\n", buffer);


    
    strcpy(password, "");
    printf("Informe o Password:");
    scanf("%s", password);
    getchar();

    strcat(password_code,password);
    strcat(password_code, "\r\n");
    strcpy(msg, password_code);
    send(meusocket, msg, strlen(msg), 0);

    strcpy(buffer, "");
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    printf("%s\n", buffer);
        

    strcpy(msg, "PWD\r\n");
    send(meusocket, msg, strlen(msg), 0);

    strcpy(buffer, "");
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    printf("%s\n", buffer);

    strcpy(msg, "CWD pub/\r\n");
    send(meusocket, msg, strlen(msg), 0);

    strcpy(buffer, "");
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    printf("%s\n", buffer);

    strcpy(msg, "PWD\r\n");
    send(meusocket, msg, strlen(msg), 0);

    strcpy(buffer, "");
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    printf("%s\n", buffer);

    strcpy(msg, "RETR readme.txt\r\n");
    send(meusocket, msg, strlen(msg), 0);

    strcpy(buffer, "");
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    printf("%s\n", buffer);
    
    
    // strcpy(msg, "QUIT\r\n");
    // send(meusocket, msg, strlen(msg), 0);
    // terminando a mensagem
    // buffer[tamanho] = '\0';

    // printf("Hora certa: %s\n", buffer);

    // termina o socket
    close(meusocket);

    return 0;
} 

// Instrucoes para compilacao
// gcc cliente.c -o cliente

