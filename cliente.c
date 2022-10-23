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
    char buffer[MAXIMOMSG + 1]; // para incluir o terminador nulo
    int tamanho, meusocket;
    struct sockaddr_in destinatario;
    struct hostent *host_info;
    struct in_addr *address;

    meusocket = socket(AF_INET, SOCK_STREAM, 0);

    destinatario.sin_family = AF_INET;
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
    
    // ip do servidor - 127.0.0.1 se estiver rodando na sua mesma maquina

    destinatario.sin_addr.s_addr = inet_addr(inet_ntoa(*address));
    // porta do servidor
    destinatario.sin_port = htons(21);

    // zerando o resto da estrutura
    memset(&(destinatario.sin_zero), '\0', sizeof(destinatario.sin_zero));

    // 3-way handshake
    connect(meusocket, (struct sockaddr *)&destinatario, sizeof(struct sockaddr));

    // recebendo resposta do servidor
    // 220
    printf("Conectado ao servidor %s (%s) na porta 21\n", argv[1], inet_ntoa(*address));
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    buffer[tamanho] = '\0';
    printf("Resposta: %s\n", buffer);

    // USER
    char username[100];
    char username_code[100] = "USER ";
    char msg[100];
    
    strcpy(username, "");
    printf("Informe o Username:");
    scanf("%s", username);
    getchar();

    strcat(username_code, username);
    strcat(username_code, "\r\n");
    strcpy(msg, username_code);
    send(meusocket, msg, strlen(msg), 0);

    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    buffer[tamanho] = '\0';
    printf("Resposta: %s\n", buffer);

    //PASSWORD
    
    char password[100];
    char password_code[100] = "PASS ";

    strcpy(password, "");
    printf("Informe o Password:");
    scanf("%s", password);
    getchar();

    strcat(password_code, password);
    strcat(password_code, "\r\n");
    strcpy(msg, password_code);
    send(meusocket, msg, strlen(msg), 0);
   
    tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
    buffer[tamanho] = '\0';
    printf("Resposta: %s\n", buffer);

    //COMANDS


    char comando;
    printf("///////////////////////////////\n");
    printf("///                         ///\n");
    printf("///    Lista de Comandos    ///\n");
    printf("///                         ///\n");
    printf("///        1. PWD           ///\n");
    printf("///        2. CWD           ///\n");
    printf("///        3. PASV          ///\n");
    printf("///        4. LIST          ///\n");
    printf("///        5. RETR          ///\n");
    printf("///        6. QUIT          ///\n");
    printf("///                         ///\n");
    printf("///////////////////////////////\n");
    printf("Informe o Comando que deseja:");
    scanf("%c", &comando);
    getchar();


    if(comando == '1'){
        //PWD

        strcpy(buffer, "PWD\r\n");
        send(meusocket, buffer, strlen(buffer), 0);

        strcpy(buffer, "");
        tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
        printf("%s\n", buffer);
        

    }else if (comando == '2'){
        //CWD
        char diretorio[100];
        char diretorio_code[100] = "CWD ";

        strcpy(diretorio, "");
        printf("Informe o Diretorio que desejas ir (ex: nomediretório/):");
        scanf("%s", diretorio);
        getchar();

        strcat(diretorio_code, diretorio);
        strcat(diretorio_code, "\r\n");
        strcpy(msg, diretorio_code);
        send(meusocket, msg, strlen(msg), 0);

        strcpy(buffer, "");
        tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
        printf("%s\n", buffer);


    }else if(comando == '3'){
        //PASV

        strcpy(buffer, "PASV\r\n");
        send(meusocket, buffer, strlen(buffer), 0);

        strcpy(buffer, "");
        tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
        printf("%s\n", buffer);


    }else if(comando == '4'){
        //LIST

        strcpy(buffer, "LIST\r\n");
        send(meusocket, buffer, strlen(buffer), 0);

        strcpy(buffer, "");
        tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
        printf("%s\n", buffer);


    }else if (comando == '5'){
        //RETR
        char download[100];
        char download_code[100] = "RETR ";

        strcpy(download, "");
        printf("Informe o Arquivo que desejas baixar (ex: readme.txt):");
        scanf("%s", download);
        getchar();

        strcat(download_code, download);
        strcat(download_code, "\r\n");
        strcpy(msg, download_code);
        send(meusocket, msg, strlen(msg), 0);

        strcpy(buffer, "");
        tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
        printf("%s\n", buffer);
    
    
    }else if (comando == '6'){
        //QUIT

        strcpy(buffer, "QUIT\r\n");
        send(meusocket, buffer, strlen(buffer), 0);
        
        tamanho = recv(meusocket, buffer, MAXIMOMSG, 0);
        buffer[tamanho] = '\0';
        printf("resposta: %s\n", buffer);
    }


    printf("Pulei pra cá");


    // termina o socket
    close(meusocket);

    return 0;
} 

// Instrucoes para compilacao
// gcc cliente.c -o cliente

