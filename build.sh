#!/bin/bash


rm -rf build/

mkdir build
cd build
cmake ..

cd ..
cmake --build build

cp make_graphics.py build/make_graphics.py
cat run.sh >> ./build/run.sh 
cd build
chmod +x run.sh
