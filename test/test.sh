#!/bin/bash

BASEDIR=`dirname $0`
PROJECT_PATH=`cd $BASEDIR; pwd`

echo "Using path:$PROJECT_PATH"

cd $PROJECT_PATH

g++ -g -Og -std=c++17 -I$PROJECT_PATH/.. -o test.vector2d.bin ./test.vector2d.cpp
./test.vector2d.bin

g++ -g -Og -std=c++17 -I$PROJECT_PATH/.. -o test.orgraph.bin ./test.orgraph.cpp
./test.orgraph.bin
