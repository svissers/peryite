#!/bin/bash
export LD_LIBRARY_PATH=~/boost_1_66_0/stage/lib/:/opt/Qt/5.10.1/gcc_64/lib
make clean
make configure
make all
make install
make test
#cd cmake-build-release/installed/bin/
#python3 runSimple.py
