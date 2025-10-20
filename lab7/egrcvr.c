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
	char sometext[BUFSIZ];
};
int main(){
	int running = 1;
	struct mymsgst somedata;
	long int msgtorecv = 0;
	int msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	while(running){
		if(msgrcv(msgid, (void*)&somedata, BUFSIZ, msgtorecv, 0) == -1){
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}
		printf("You wrote: %s", somedata.sometext);
		if(strncmp(somedata.sometext, "end", 3) == 0) running = 0;
	}
	if(msgctl(msgid, IPC_RMID, 0) == -1){
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}