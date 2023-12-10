#!/bin/bash
set -eu

gcc -o mmap_reading ../mmap_reading.c
gcc -o read_reading ../read_reading.c

sudo -v

for chunk_size in $(seq 8192 1024 65536)
do
  echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null

  echo -n "$chunk_size,"
  ./mmap_reading ../sample-files/40GiB_file "$chunk_size"

  echo 3 | sudo tee /proc/sys/vm/drop_caches > /dev/null

  echo -n ","
  ./read_reading ../sample-files/40GiB_file "$chunk_size"

  echo
done

rm mmap_reading read_reading
