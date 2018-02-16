#!/bin/bash

make all
make install
cd cmake-build-release/installed/bin/
python3 runSimple.py
