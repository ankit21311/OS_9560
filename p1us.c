#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "unix_sock.server"

char* retRandomString(){
	char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *strGenerated = (char*)calloc(5, sizeof(char));

	int k = 0 ;
	while(k<4){
		strGenerated[k] = letters[rand()%(52)];
		k++ ;
	}

	return strGenerated;
}

char* copyString(char *src, int start){
	char *ptr = (char*)calloc(26, sizeof(char));
	ptr[25] = '\0';

	int a = start ;
	while(a<start+25){
		ptr[a%25] = src[a];
		a++ ;
	}
	return ptr;
}

void pasteString(char* dest, char *src, int fromPos){

	int k = 0 ;
	while(k<5){
		dest[fromPos+k] = src[k];
		k++ ;
	}

}

int main(int argc, char *argv[]){
    int server_sock, client_sock, len, rc;
    int bytes_rec = 0;
    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;
    char *buffer = (char*)calloc(26, sizeof(char));
    int backlog = 1;
    memset(&server_sockaddr, 0, sizeof(struct sockaddr_un));
    memset(&client_sockaddr, 0, sizeof(struct sockaddr_un));
    



    srand(time(NULL));
	int loc4paste = 0;
	int loc4copy = 0;
	int intRecieved;
	char* strings = (char*)calloc(251, sizeof(char));
	strings[250]= '\0';
	int help = 65;

	for(int i = 0; i<50; i++){
		char *ptr1 = retRandomString();
		ptr1[4] = help;
		pasteString(strings, ptr1, loc4paste);
		loc4paste = loc4copy + 5;
		help++;
		free(ptr1);
	}    


	server_sock = socket (AF_UNIX, SOCK_SEQPACKET, 0);
	if(server_sock<0){
		perror("Failed to Create the sockets\n");
		exit(1);
	}


    server_sockaddr.sun_family = AF_UNIX;
    strcpy(server_sockaddr.sun_path, SOCK_PATH);
    len = sizeof(server_sockaddr);

    unlink(SOCK_PATH);

    if(bind(server_sock, (struct sockaddr *)&server_sockaddr, len)<0){
		perror("Bind failed\n");
		exit(1);
	}

    if(listen(server_sock, backlog)<0){
        printf("Can't Listen To Client\n");
        exit(1);
    }

    client_sock = accept(server_sock, (struct sockaddr *) &client_sockaddr, &len);

    
    if(client_sock==-1){
        printf("Cant Accept the client\n");
        exit(1);
    }

	

    len = sizeof(client_sockaddr);
    rc = getpeername(client_sock, (struct sockaddr *) &client_sockaddr, &len);
    if(rc==-1){
        printf("can't get the peer name\n");
        close(server_sock);
        close(client_sock);
        exit(1);
    }
    

    memset(buffer, 0, 26);
    
    
    for(int k = 0; k<10; k++){
        buffer = copyString(strings, loc4copy);
		loc4copy += 25;
		if(send(client_sock, buffer, 26, 0)<0){
			perror("Send failed\n");
			close(client_sock);
			exit(1);
		}
		sleep(1);
		if(recv(client_sock, &intRecieved, sizeof(int), 0)<0)  
 			perror("reading stream message error");
 		else
			printf("number recieved for the acknowledgement:%d\n", intRecieved);
        free(buffer);
	}


    close(server_sock);
    close(client_sock);
    return 0;
}