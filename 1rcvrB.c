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

int ispalin(int num){
	int rev = 0, og = num, digi;
	while(num > 0){
		digi = num % 10;
		rev = rev * 10 + digi;
		num /= 10;
	}
	return (og == rev);
}
int main(){
	int running = 1;
	struct mymsgst numstruct;
	int msgid = msgget((key_t)1234, 0666 | IPC_CREAT);
	if(msgid == -1){
		fprintf(stderr, "msgget failed with error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	while(running){
		if(msgrcv(msgid, (void*)&numstruct, sizeof(numstruct.number), 0, 0) == -1){
			perror("msgrcv failed");
			exit(EXIT_FAILURE);
		}
		if(strncmp(numstruct.number, "end", 3) == 0) running = 0;
		else{
			int num = atoi(numstruct.number);
			if(ispalin(num)) printf("%d: palindrome\n", num);
			else printf("%d: not a palindrome\n", num);
		}
	}
	if(msgctl(msgid, IPC_RMID, 0) == -1){
		fprintf(stderr, "msgctl(IPC_RMID) failed\n");
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}