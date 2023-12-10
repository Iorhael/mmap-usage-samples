#!/bin/bash
gcc -o shm_initializer ../readers-writers-unrelated/shm_initializer.c
gcc -o reader_writer ../readers-writers-unrelated/reader_writer.c
gcc -o shm_unlinker ../readers-writers-unrelated/shm_unlinker.c

echo -n "Contents of the file before modification: "
cat ../sample-files/shared.txt
echo

./shm_initializer /shared_memory

./reader_writer ../sample-files/shared.txt /shared_memory writer &
sleep 0.1
./reader_writer ../sample-files/shared.txt /shared_memory reader &
./reader_writer ../sample-files/shared.txt /shared_memory reader &

wait

./shm_unlinker /shared_memory

echo -n "Contents of the file after modification: "
cat ../sample-files/shared.txt
echo

sed -i "s/./A/g" ../sample-files/shared.txt

rm shm_initializer reader_writer shm_unlinker