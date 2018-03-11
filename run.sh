#!/bin/bash
export LD_LIBRARY_PATH=~/lorin/boost_1_66_0/stage/lib/
make clean
make configure
make all
make install
make test
#cd cmake-build-release/installed/bin/
#python3 runSimple.py
