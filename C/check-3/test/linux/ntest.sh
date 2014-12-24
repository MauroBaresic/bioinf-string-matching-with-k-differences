#!/bin/bash

data_path="../../../data/Texts"
output_file="outputPerformanceN"
pattern_file="pattern.fa"
k=4

./lvn3 "${data_path}/${pattern_file}" "${data_path}/text5e1.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text25e1.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text1e3.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text5e3.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text1e4.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text5e4.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text1e5.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text5e5.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text1e6.fa" $k "${output_file}"
./lvn3 "${data_path}/${pattern_file}" "${data_path}/text2e6.fa" $k "${output_file}"