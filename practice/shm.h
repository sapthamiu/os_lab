#define MAX 1024
#define SHM_KEY 1234
typedef struct shm_msg{
    int written;
    char data[MAX];
}SHM_MSG;