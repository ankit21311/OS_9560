#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>



struct msgContainer{
    long int message_var;
    char msg[26];
};

struct msgContainerForInt{
    long int message_var;
    char msgI[2];
};

int main(int argc, char *argv[]){
    struct msgContainer myMsg;
    struct msgContainerForInt myInt;
    
    myInt.message_var = 1;

    int msgid = msgget((key_t)1234, 0777|IPC_CREAT);
    int msgidForInt = msgget((key_t)12347, 0777|IPC_CREAT);
    if(msgid==-1 || msgidForInt==-1){
        printf("Error in Creating the queue\n");
    }
    long int msgRecpriority = 0;

    int maxInd = -1;
    int help = 65;
	for(int k = 0; k<10; k++){
        msgrcv(msgid, (void *)&myMsg, 26, msgRecpriority, 0);

		int i = 0;
		printf("---String Set No:%d---\n",k+1);
        printf("\n") ;
		while(i<26){
			if((i+1)%5 != 0 || i==0){
				printf("%c", myMsg.msg[i]);
			}else{
				
				if(maxInd<(int)myMsg.msg[i]){
					maxInd = (int)myMsg.msg[i]-65;
				}
                printf(", ID:%d\n", maxInd);
			}
			i++;
		}
		printf("---------------------------------\n");
        printf("") ;
        
        char temp[2];
        temp[0] = (char)(maxInd+help);
        temp[1] = '\0';
        strcpy(myInt.msgI, temp);
        if(msgsnd(msgidForInt, (void *)&myInt, 2, 0)==-1){
            printf("Messasg not sent\n");
        } 
    }
    
    msgctl(msgid, IPC_RMID, 0);
    return 0;
}
