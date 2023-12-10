#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../readers_writers.h"

int main(int argc, char **argv){
    int                     shm_fd;
    const char              *shmpath = argv[1];
    struct shared_variables *struct_memory;

    // Create shared memory object and set its size to the size of our structure.
    shm_fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shm_fd == -1){
        perror("shm open");
        return 1;
    }

    // Truncate new shared object memory.
    if (ftruncate(shm_fd, sizeof(struct shared_variables)) == -1){
        perror("ftruncate");
        return 1;
    }

    // Map the shared object into the caller's address space.
    struct_memory = mmap(NULL, sizeof(*struct_memory), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (struct_memory == MAP_FAILED){
        perror("mmap");
        return 1;
    }

    // shared variable initialization
    struct_memory -> readers_count = 0;

    pthread_mutexattr_t attrmutex;
    pthread_mutexattr_init(&attrmutex);
    pthread_mutexattr_setpshared(&attrmutex, PTHREAD_PROCESS_SHARED);

    pthread_mutex_init(&(struct_memory -> readers_mutex), &attrmutex);
    pthread_mutex_init(&(struct_memory -> file_mutex), &attrmutex);

    munmap(struct_memory, sizeof(*struct_memory));

    return 0;
}