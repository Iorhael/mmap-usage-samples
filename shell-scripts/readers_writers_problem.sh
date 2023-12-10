#!/bin/bash
gcc -o readers_writers_problem ../readers_writers_problem.c

echo -n "Contents of the file before modification: "
cat ../sample-files/shared.txt
echo

./readers_writers_problem ../sample-files/shared.txt 0x1 # 0x1 - MAP_SHARED, 0x2 - MAP_PRIVATE

echo -n "Contents of the file after modification: "
cat ../sample-files/shared.txt
echo

sed -i "s/./A/g" ../sample-files/shared.txt

rm readers_writers_problem