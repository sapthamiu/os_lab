#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

struct mymsgst{
	long int mymsgtype;
	char number[20];
};
int main(){
	int running = 1;
	struct mymsgst numstruct;
	int msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	while(running){
		printf("Enter a no.: ");
		fgets(numstruct.number, 20, stdin);
		numstruct.mymsgtype = 1;
		if(msgsnd(msgid, (void*)&numstruct, sizeof(numstruct.number), 0) == -1){
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		if(strncmp(numstruct.number, "end", 3) == 0) running = 0;
	}
	exit(EXIT_SUCCESS);
}