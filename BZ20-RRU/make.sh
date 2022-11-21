#!/bin/bash

cd /root/BZ_AUTOTEST/BZ20-RRU && make clean
cd /root/BZ_AUTOTEST/BZ20-RRU && source setenv.sh && make -j12
cd /root/BZ_AUTOTEST/BZ20-RRU
