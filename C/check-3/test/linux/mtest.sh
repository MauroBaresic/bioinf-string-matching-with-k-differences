#!/bin/bash

data_path="../../../data/Test data/MTest"
output_file="outputPerformanceM"
text_file="text.fa"
pattern_name="pattern"
pattern_ext=".fa"
k_file="kVal.txt"

k=`cat "${data_path}/${k_file}"`

for i in {0..9}
do
    ./lvn3 "${data_path}/${pattern_name}$i${pattern_ext}" "${data_path}/${text_file}" $k "${output_file}"
done