#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>


int main(int argc, char** argv) {
    const char  *file_path = argv[1];
    int         flag = (int)strtol(argv[2], NULL, 16);
    struct stat file_info;

    // We open the file for read/write operation
    int fd = open(file_path, O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (fstat(fd, &file_info) < 0) {
        perror("fstat problems");
        return 1;
    }

    // Create a memory-mapped region for the file.
    void *file_memory = mmap(NULL, file_info.st_size, PROT_READ | PROT_WRITE,flag, fd, 0);
    close(fd);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }

    pid_t pid = fork();

    if(pid < 0) {
        perror("fork");
        return 1;
    }

    if(pid == 0){ // Child process.
        for (size_t i = 0; i < file_info.st_size; i++) {
            strncpy(file_memory + i, "B", 1);
            printf("\33[38;5;%imWriter %d write: B\33[0m\n", 13 + (getpid() * 5) % 242, getpid());
            sleep(1);
        }
        // printf("\33[38;5;%imContents of the memory-mapped region: %s\33[0m\n",13 + (getpid() * 5) % 242, (char *) file_memory);
        return 0;
    }else{ // Parent process.
        for (size_t i = 0; i < file_info.st_size; i++) {
            printf("\33[38;5;%imReader %d read: %.*s\33[0m\n", 13 + (getpid() * 5) % 242, getpid(), 1, (char *)(file_memory + 1));
            sleep(1);
        }
        wait(NULL);
    }

    // Unmap the memory.
    munmap(file_memory, file_info.st_size);

    return 0;
}
