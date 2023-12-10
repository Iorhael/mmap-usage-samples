#!/bin/bash
gcc -o readers_writers_related ../readers_writers_related.c

echo -n "Contents of the file before modification: "
cat ../sample-files/shared.txt
echo

./readers_writers_related ../sample-files/shared.txt /shared_memory

echo -n "Contents of the file after modification: "
cat ../sample-files/shared.txt
echo

sed -i "s/./A/g" ../sample-files/shared.txt

rm readers_writers_related