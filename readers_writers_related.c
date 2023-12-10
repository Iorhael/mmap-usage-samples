#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <sys/wait.h>
#include "readers_writers.h"

// Readers-preference solution.

void reader(struct stat file_info, void *file_memory, struct shared_variables *struct_memory) {
    pthread_mutex_lock(&(struct_memory->readers_mutex));
    struct_memory->readers_count += 1;
    if (struct_memory->readers_count == 1) pthread_mutex_lock(&(struct_memory->file_mutex));
    pthread_mutex_unlock(&(struct_memory->readers_mutex));

    for (size_t i = 0; i < file_info.st_size; i++) {
        printf("\33[38;5;%imReader %d read: %.*s\33[0m\n", 13 + (getpid() * 5) % 242, getpid(), 1, (char *)(file_memory + 1));
        sleep(1);
    }

    pthread_mutex_lock(&(struct_memory->readers_mutex));
    struct_memory->readers_count -= 1;
    if (struct_memory->readers_count == 0) pthread_mutex_unlock(&(struct_memory->file_mutex));
    pthread_mutex_unlock(&(struct_memory->readers_mutex));
}

void writer(struct stat file_info, void *file_memory, struct shared_variables *struct_memory) {
    pthread_mutex_lock(&(struct_memory->file_mutex));

    for (size_t i = 0; i < file_info.st_size; i++) {
        strncpy(file_memory + i, "B", 1);
        printf("\33[38;5;%imWriter %d write: B\33[0m\n", 13 + (getpid() * 5) % 242, getpid());
        sleep(1);
    }

    pthread_mutex_unlock(&(struct_memory->file_mutex));
}


int main(int argc, char **argv) {
    const char              *file_path = argv[1];
    const char              *shmpath = argv[2];
    struct stat             file_info;
    int                     shm_fd;
    struct shared_variables *struct_memory;

    // We open the file for read/write operation
    int fd = open(file_path, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    shm_fd = shm_open(shmpath, O_CREAT | O_EXCL | O_RDWR, 0600);
    if (shm_fd == -1){
        perror("shm open");
        return 1;
    }

    if (fstat(fd, &file_info) < 0) {
        perror("fstat problems");
        return 1;
    }

    // Create a memory-mapped region for the file.
    void *file_memory = mmap(NULL, file_info.st_size, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 0);
    close(fd);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
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

    pid_t pid_1 = fork();

    if (pid_1 < 0) {
        perror("fork");
        return 1;
    }

    if (pid_1 == 0) { // Child process 1.
        writer(file_info, file_memory, struct_memory);
        return 0;
    } else {
        pid_t pid_2 = fork();

        if (pid_2 < 0) {
            perror("fork");
            return 1;
        }

        if (pid_2 == 0) { // Child process 2.
            reader(file_info, file_memory, struct_memory);
            return 0;
        } else { // Parent process.
            reader(file_info, file_memory, struct_memory);
            waitpid(pid_1, NULL, 0);
            waitpid(pid_2, NULL, 0);
        }
    }

    // Unlink shared memory object.
    shm_unlink(shmpath);

    // Unmap the memory.
    munmap(file_memory, file_info.st_size);
    munmap(struct_memory, sizeof(*struct_memory));

    return 0;
}
