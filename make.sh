#!/bin/bash
path_current=$(pwd)
code_path="/BZ20-RRU"
make_path="$path_current$code_path"
cd $make_path && make clean
cd $make_path && source setenv.sh
cd $make_path && make -j12
