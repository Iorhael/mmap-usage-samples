#include <pthread.h>

#ifndef MEMORY_MAPPING_READERS_WRITERS_H
#define MEMORY_MAPPING_READERS_WRITERS_H

struct shared_variables{
    pthread_mutex_t readers_mutex;
    int             readers_count;
    pthread_mutex_t file_mutex;
};

#endif //MEMORY_MAPPING_READERS_WRITERS_H
