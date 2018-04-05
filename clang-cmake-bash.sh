#!/bin/sh
clear 
echo "Running first test."
cmake . -Bbin/clang-mac -G "Unix Makefiles" -DCMAKE_C_COMPILER="/usr/bin/clang" -DCMAKE_CXX_COMPILER="/usr/bin/clang++"
