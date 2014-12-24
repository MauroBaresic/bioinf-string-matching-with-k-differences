#!/bin/bash

data_path="../../../data/Test data/KTest"
output_file="outputPerformanceK"
pattern_file="pattern.fa"
text_file="text.fa"
k_file="kVals.txt"

for k in `cat "${data_path}/${k_file}"`
do
    ./lvn3 "${data_path}/${pattern_file}" "${data_path}/${text_file}" $k "${output_file}"
done