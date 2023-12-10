#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    const char *file_path = "../sample-files/line_by_line.txt";
    struct stat file_info;

    // Open the file for read operation.
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

    void *start = file_memory;
    void *current = file_memory;

    printf("Offset (bytes) | Text\n");

    for (;;) {
        printf("% 14d | ", (int)(current - file_memory));
        while (*((char *)current) != '\n' && current < file_memory + file_info.st_size) {
            ++current;
        }

        printf("%.*s\n", (int)(current - start), (char *)start);
        if (current == file_memory + file_info.st_size) break;
        start = current + 1;
        current = start;
    }

    // Unmap the memory.
    munmap(file_memory, file_info.st_size);

    return 0;
}
