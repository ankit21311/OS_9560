#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/msg.h>




struct msgContainer{
    long int msg_type;
    char msg[26];
};



struct msgContainerForInt{
    long int msg_type;
    char msgI[2];
};

char* retRandomString(){
	char letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char *strGenerated = (char*)calloc(5, sizeof(char));

	int p = 0 ;
	while(p<4){
		strGenerated[p] = letters[rans()%(52)] ;
		p++ ;
	}

	return strGenerated;
}


char* copyString(char *src, int start){
	char *ptr = (char*)calloc(26, sizeof(char));
	ptr[25] = '\0';

	int k = start ;
	while(k<start+25){
		ptr[k%25] = src[k] ;
		k++ ;
	}

	return ptr;
}

void pasteString(char* dest, char *src, int fromPos){

	int m = 0 ;
	while(m<=4){
		dest[fromPos+m] = src[m];
		m++ ;
	}
	
}



int main(int argc, char *argv[]){

    srand(time(NULL));
	int loc4paste = 0;
	int loc4copy = 0;
	int intRecieved;
	char* strings = (char*)calloc(251, sizeof(char));
	strings[250]= '\0';

    int help = 65;




    for(int s = 0; s<50; s++){
		char *ptr1 = retRandomString();
		ptr1[4] = help;
		pasteString(strings, ptr1, loc4paste);
		loc4paste += 5;
		help++;
		free(ptr1);
	}


    struct msgContainer myMsg;
    struct msgContainerForInt myInt;



    long int msgRecpriority = 0;
    int msgid = msgget((key_t)1234, 0777|IPC_CREAT);
    int msgidForInt = msgget((key_t)12347, 0777|IPC_CREAT);



    if(msgid==-1 || msgidForInt ==-1){
        printf("Error in Creating the queue\n");
    }
    myMsg.msg_type = 1;

	for(int k = 0; k<10; k++){

		char *buffer = copyString(strings, loc4copy);
		loc4copy = loc4copy + 25;

        strcpy(myMsg.msg, buffer);
        if(msgsnd(msgid, (void *)&myMsg, 26, 0)==-1){
            printf("Messag not sent\n");
        }        
        
		msgrcv(msgidForInt, (void *)&myInt, 2, msgRecpriority, 0);
        printf("The Number Recieved for the Acknowledgement:%d\n", (int)myInt.msgI[0]-65);
		bzero(myInt.msgI, 2);
        bzero(myMsg.msg, 26);

       
		
	}
    msgctl(msgidForInt, IPC_RMID, 0);
    return 0;
}
