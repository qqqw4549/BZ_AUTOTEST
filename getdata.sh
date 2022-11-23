#!/bin/bash
path_current=$(pwd)
code_path="/BZ20-RRU"
make_path="$path_current$code_path"

sftp root@192.168.1.221
cd $path_current && mv data.txt /mnt/d/RRU_SDK/MATLAB/
