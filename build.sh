#!/bin/sh
set -euo pipefail
trap "cleanup" EXIT

cleanup() {
	cd ..
	rm -r build
}

mkdir build && cd build

if ! [ -x "$(command -v sdcc)" ]; then
	echo "Build script requires sdcc!" >&2
	exit 1
fi

sdcc -mmcs51 -c ../src/Delay.c -D FOSC_160000 -I../include
sdcc -mmcs51 -c ../src/Common.c -D FOSC_160000 -I../include
sdcc -mmcs51 -c ../src/main.c -D FOSC_160000 -I../include
sdcc -mmcs51 -o firmware.hex main.rel Delay.rel Common.rel
cp firmware.hex ../

exit 0

