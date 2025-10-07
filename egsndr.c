#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#define MAX 512

struct mymsgst{
	long int mymsgtype;
	char sometext[MAX];
};
int main(){
	int running = 1;
	struct mymsgst somedata;
	int msgid;
	char buffer[BUFSIZ];
	msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	while(running){
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);
		somedata.mymsgtype = 1;
		strcpy(somedata.sometext, buffer);
		if(msgsnd(msgid, (void*)&somedata, MAX, 0) == -1){
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}
		if(strncmp(buffer, "end", 3) == 0) running = 0;
	}
	exit(EXIT_SUCCESS);
}