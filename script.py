#!/usr/bin/env python
# coding=utf-8
import os
path=''

print("-----------------------------------------------")

os.system("rm tags GPATH  GRTAGS  GTAGS")

print("     create ctag                               ")

os.system("gtags")

os.system("rm cscope* ")

#os.system("cscope -Rbq")

path = str(os.getcwd())

os.system("find "+ path + " -name '*.h' > cscope.files ")
os.system("find "+ path + " -name '*.c' >> cscope.files ")

print("     create cscope                           ")

os.system("cscope -bqk -i cscope.files ")

print("-----------------------------------------------")
