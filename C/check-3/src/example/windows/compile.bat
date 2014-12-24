@echo off

set src_path=../..

start /wait gcc -o lvn3 "%src_path%/functions.c" "%src_path%/lvn3.c"