@echo off

set data_path=../../../data/Test data/KTest
set output_file=outputPerformanceK
set pattern_file=pattern.fa
set text_file=text.fa
set k_file=kVals.txt

for /f "usebackq delims=" %%t in ("%data_path%/%k_file%") do (
    set tokens=%%t
)

for %%n in (%tokens%) do (
    call :getNum %%n
)
goto :EOF

:getNum
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/%text_file%" %1 "%output_file%"
goto :EOF

:EOF