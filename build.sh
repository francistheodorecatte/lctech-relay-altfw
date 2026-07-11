#!/bin/sh
set -euo pipefail

mkdir build && cd build
sdcc -mmcs51 -c ../src/Delay.c -D FOSC_160000 -I../include
sdcc -mmcs51 -c ../src/Common.c -D FOSC_160000 -I../include
sdcc -mmcs51 -c ../src/main.c -D FOSC_160000 -I../include
sdcc -mmcs51 -o firmware.hex main.rel Delay.rel Common.rel -D FOSC_160000 -I../include
cp firmware.hex ../ && cd .. && rm -r build

