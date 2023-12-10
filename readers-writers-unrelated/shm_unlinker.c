#include <sys/mman.h>
#include <stdio.h>

int main(int argc, char **argv){
    // Unlink shared object memory.
    if(shm_unlink(argv[1]) == -1){
        perror("shm unlink");
        return 1;
    }

    return 0;
}