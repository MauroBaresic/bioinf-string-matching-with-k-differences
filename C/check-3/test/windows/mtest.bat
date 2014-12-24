@echo off

set data_path=../../../data/Test data/MTest
set output_file=outputPerformanceM
set text_file=text.fa
set pattern_name=pattern
set pattern_ext=.fa
set k_file=kVal.txt

for /f "usebackq delims=" %%t in ("%data_path%/%k_file%") do (
    set k=%%t
)

for /l %%i in (0,1,9) do (
    start /wait lvn3.exe "%data_path%/%pattern_name%%%i%pattern_ext%" "%data_path%/%text_file%" %k% "%output_file%"
)