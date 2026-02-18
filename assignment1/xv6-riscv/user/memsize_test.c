#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    
    printf("The running process is using %d bytes of memory\n",memsize());
    int size = 20000;
    void *ptr = malloc(size*sizeof(char));
    printf("After the allocation the running process is using %d bytes of memory\n", memsize());
    free(ptr);
    printf("After the release the running process is using %d bytes of memory\n", memsize());
    
    exit(0,"");
}