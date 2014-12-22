#!/bin/bash

src_path="../../src"

gcc -o lvn3 ${src_path}/test.c ${src_path}/functions.c ${src_path}/lvn3.c -include "${src_path}/test.h"