#include<unistd.h>
#include<stdlib.h>
int main(){
	if(write(1, "Here is some data\n", 18) != 18)
		write(2, "A write error has occurred on fd 1\n", 35);
	exit(0);
}