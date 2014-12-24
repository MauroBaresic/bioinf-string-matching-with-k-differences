@echo off

set data_path=../../../data/Texts
set output_file=outputPerformanceN
set pattern_file=pattern.fa
set k=4

start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text5e1.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text25e1.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text1e3.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text5e3.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text1e4.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text5e4.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text1e5.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text5e5.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text1e6.fa" %k% "%output_file%"
start /wait lvn3.exe "%data_path%/%pattern_file%" "%data_path%/text2e6.fa" %k% "%output_file%"