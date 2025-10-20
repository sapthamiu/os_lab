#include<unistd.h>
#include<stdlib.h>
int main(){
    char buf[128];
    // write(1, "Enter something\n", 16);
    int n = read(0, buf, 128);
    if(n == -1)
        write(2, "A read error has occurred\n", 26);
        if(write(1, buf, n) != n)
            write(2, "A write error has occurred\n", 27);
    exit(0);
}