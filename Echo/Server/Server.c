//Server to be run on Linux machine compile and link with pthread library . -lpthread
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <pthread.h>

void *connection_handler(void *);

int main(){
	int msg_counter = 0;

	char rcv_buff[256]  = {'0'};
	char clnt_msg[256]  = {'0'};
	char send_buff[256] = {'0'};

	int serv_socket, clnt_socket, *new_sock, cunk, rcv_size = 0;

	//server sockaddr
	struct sockaddr_in server, client;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_family = AF_INET;
	server.sin_port = htons(3210);


	if((serv_socket = socket(AF_INET, SOCK_STREAM, 0))== -1){
		printf("Could not create socket\n");
		return -1;

	}else if(bind(serv_socket, (struct sockaddr *)&server, sizeof(server))<0){
		printf("Binding failed\n");
		close(serv_socket);
		return -2;

	}else{
		listen(serv_socket, 3);
	}

	cunk = sizeof(struct sockaddr_in);
	while((clnt_socket = accept(serv_socket, (struct sockaddr *)&client, (socklen_t *)&cunk))){

		pthread_t new_thread;
		new_sock = malloc(1);
		*new_sock = clnt_socket;

		if(pthread_create(&new_thread, NULL, connection_handler, (void *)new_sock)<0){
			perror("Could not create thread\n");
			return -1;
		}

		//pthread_join(new_thread, NULL); //process threads one by one
	}

	if(clnt_socket < 0){
		perror("Accept Failed\n");
		return -1;
	}


	close(clnt_socket);
	return 0;
}

void *connection_handler(void *socket_desc){

	int clnt_socket = *(int *)socket_desc;
	char rcv_buff[256]  = {'0'};
	char send_buff[256] = {'0'};
	int rcv_size = 0;

	while(rcv_size = recv(clnt_socket, rcv_buff, sizeof(rcv_buff) - 3, 0)){
		rcv_buff[rcv_size] = '\r';
		rcv_buff[rcv_size] = '\n';
		rcv_buff[rcv_size+1] = '\0';

		if(strlen(rcv_buff) == 0){
			break;
		}

		strcpy(send_buff, rcv_buff);
		write(clnt_socket, send_buff, strlen(send_buff));

		printf("%s", rcv_buff);
		fflush(stdout);
		memset(rcv_buff, 0, sizeof(rcv_buff));
	}

	free(socket_desc);
	return 0;
}
