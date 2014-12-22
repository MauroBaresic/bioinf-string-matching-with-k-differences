@ECHO OFF

set src_path="../.."

START /WAIT gcc -o lvn3 %src_path%/functions.c %src_path%/lvn3.c