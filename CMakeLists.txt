cmake_minimum_required(VERSION 3.26)
project(memory_mapping C)

set(CMAKE_C_STANDARD 11)

add_executable(line_by_line_reading line_by_line_reading.c)
add_executable(mmap_reading mmap_reading.c)
add_executable(read_reading read_reading.c)
# reader-writers problem
add_executable(readers_writers_problem readers_writers_problem.c)
add_executable(readers_writers_related readers_writers_related.c)
# readers-writers-unrelated
add_executable(shm_initializer readers-writers-unrelated/shm_initializer.c)
add_executable(reader_writer readers-writers-unrelated/reader_writer.c)
add_executable(shm_unlinker readers-writers-unrelated/shm_unlinker.c)