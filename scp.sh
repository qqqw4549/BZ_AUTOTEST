#!/bin/bash
path_current=$(pwd)
app_path_="/BZ20-RRU/app/bin"
app_path="$path_current$app_path_"

rm ~/.ssh/known_hosts
cd $app_path && scp * root@192.168.1.221:/home/root
