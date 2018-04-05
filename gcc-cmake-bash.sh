#!/bin/sh
clear
echo "Running first test."
cmake src -Bbin/gcc-mac -G "Unix Makefiles" -DCMAKE_C_COMPILER="/usr/bin/gcc" -DCMAKE_CXX_COMPILER="/usr/bin/g++"

