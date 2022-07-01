#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int sockfd;
char client_message[256];
char server_ip[256];
int port;
struct sockaddr_in serv_addr;//server address
socklen_t sLen = sizeof(serv_addr);


void create_udp_server(int port, char* message, const char* comms_ip){
    memset(&client_message, '\0', sizeof(client_message));
    memcpy(&client_message, message, strlen(message)); //copy the argv[2] which is message int this case to the client_message
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_port        = htons(port); //htons means convert a number into 16-bit network representation
    serv_addr.sin_addr.s_addr = inet_addr(comms_ip);

    //Create socket here
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0){  // int sockfd = socket(domain, type, protocol) and AF_INET uses IPv4 and SOCK_DGRAM uses Datagram Protocol=UDP
        printf("Cannot create socket\n");
        exit(-1);   
    }
    printf("Socket created successfully and message is ready to be delivered!\n");
    //SEND MESSAGE HERE
    
    if(sendto(sockfd, client_message , strlen(client_message), 0, (struct sockaddr*)&serv_addr, sLen)<0){
        printf("Cannot send the message!\n");
        exit(-1);
    }
    printf("Client message: %s",client_message);
    close(sockfd);
    exit(0);
}

void load_server_configuration(){
    char line[256];
    FILE* config_file = fopen("starter.config", "r");
    
    char* parameter;
    char* value;
    char* token;
    const char* delimiter_characters = " ";

    if(config_file == NULL)
    {
        fprintf( stderr, "Unable to open the config file");
    }
    else
    {
        fgets(line, 256, config_file);
        int count = 0;
        token = strtok(line, delimiter_characters);
        while(token != NULL && count != 2)
        {
            if(count != 0){
                token = strtok(NULL,delimiter_characters);
            }
            strcpy(parameter, token);
            token = strtok(NULL,delimiter_characters);
            strcpy(value, token);
            
            if(strcmp(parameter,"SERVER_IP")==0){
                strcpy(server_ip, value);
            }
            else if(strcmp(parameter,"PORT")==0){
                sscanf(value, "%d", &port);
            }
            count++;
        }
    } 
    printf("Server ip: %s\n", server_ip);
    printf("Port: %d\n", port);  
}

int main(){
    char *message = "Hello from c version of my udp implementation\n";
    load_server_configuration();
    create_udp_server(port, message, server_ip);
    return 0;

}
