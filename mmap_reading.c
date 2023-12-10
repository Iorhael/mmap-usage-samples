#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    const char *file_path = argv[1];
    struct stat file_info;

    // We open the file for read operation
    int fd = open(file_path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    if (fstat(fd, &file_info) < 0) {
        perror("fstat problems");
        return 1;
    }

    // Create a memory-mapped region for the file.
    void *file_memory = mmap(NULL, file_info.st_size, PROT_READ, MAP_SHARED, fd, 0);
    close(fd);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    const int chunk_size = atoi(argv[2]);
    char buffer [chunk_size];

    clock_t start, end;

    start = clock();
    for (void *ptr = file_memory; ptr + chunk_size < file_memory + file_info.st_size; ptr += chunk_size) {
        memcpy(&buffer, ptr, chunk_size);
    }
    end = clock();
    double time_taken = (double)(end - start) / (double)CLOCKS_PER_SEC;

    // Unmap the memory.
    munmap(file_memory, file_info.st_size);
    printf("%f", time_taken);
//    printf("File size: %li bytes, chunk size: %i bytes\n", file_info.st_size, chunk_size);

    return 0;
}
