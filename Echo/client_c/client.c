//Windows client, compile and link with -lws2_32 (winsock) 

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <signal.h>

int main(int argc, char *argv[]){

    struct WSAData wsa;
    char send_buff[256]= "Initialize";
    char clnt_msg[238] = {'0'};
    char clnt_name[10] = {'0'};
    char rcv_buff[1024] = {'0'};
    char server_ip[16] = {'0'};

    sprintf(server_ip, argv[1]);
    sprintf(clnt_name,"%s", argv[2]);

    int clnt_socket;
    struct sockaddr_in server;
    int rcv_Size  = 0;
    int send_stat = 0;

    //server parameters
    server.sin_addr.S_un.S_addr = inet_addr(server_ip);
    server.sin_family = AF_INET;
    server.sin_port = htons(3210);

    if(argc < 3){
        printf("Too few arguments, exiting\nEnter <client> <IP> <user_name>");
        return 0;

    }else if(WSAStartup(0x022, &wsa)){
        printf("Winsock initialization failed\n");
        return -1;

    }else if((clnt_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET){
        printf("Could not create socket\n");
        WSACleanup();
        return -2;
        
    }else if(connect(clnt_socket, (struct sockaddr *)&server, sizeof(server)) < 0){
        printf("Connect Error\n");
        closesocket(clnt_socket);
        WSACleanup();
        return -3;

    }else{

        while(1){
            
            if(!strcmp(clnt_msg,"exit")){
                char *exit_msg = "";
                send(clnt_socket, exit_msg, strlen(exit_msg), 0);
                break;
            }
            //endif

            send_stat = send(clnt_socket, send_buff, strlen(send_buff), 0); 
            if(send_stat > 0){
                
                if((rcv_Size = recv(clnt_socket, rcv_buff, sizeof(rcv_buff), 0)) == SOCKET_ERROR){
                    printf("Receive failed\n");
                    return -5;

                }else{
                    rcv_buff[rcv_Size] = '\0';
                    printf("%s%s: ", rcv_buff, clnt_name);
                    
                    memset(rcv_buff, 0, sizeof(char)*sizeof(rcv_buff));
                    memset(clnt_msg, 0, sizeof(char)*sizeof(clnt_msg));

                    while(!(scanf("%237[^\n]s", clnt_msg)))
                        fflush(stdin);
                    
                    sprintf(send_buff,"%s: %s", clnt_name, clnt_msg);
                }

            }else{
                printf("Send failed\n");
                return -4;
            }
        }
    }

    closesocket(clnt_socket);
    WSACleanup();
    return 0;
}