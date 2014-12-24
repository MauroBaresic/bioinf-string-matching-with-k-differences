@echo off

set src_path=../../src

start /wait gcc -o lvn3 "%src_path%/test.c" "%src_path%/functions.c" "%src_path%/lvn3.c" -lpsapi -include "%src_path%/test.h"