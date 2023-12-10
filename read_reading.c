#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
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

    const int chunk_size = atoi(argv[2]);
    char buffer [chunk_size];

    clock_t start, end;

    start = clock();
    while (read(fd, &buffer, chunk_size) > 0);
    end = clock();
    double time_taken = (double)(end - start) / (double)CLOCKS_PER_SEC;

    printf("%f", time_taken);
//    printf("File size: %li bytes, chunk size: %i bytes\n", file_info.st_size, chunk_size);
    close(fd);

    return 0;
}
