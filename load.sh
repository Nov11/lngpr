#!/bin/bash
MOD=${1:- record}
rmmod ${MOD}
insmod ${MOD}.ko
lsmod | grep ${MOD} && echo "load succ" || echo "load failed"
dmesg | tail 
