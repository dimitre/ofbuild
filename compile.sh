#!/bin/zsh
cd "$(dirname "$0")"
brew install pkg-config yaml-cpp fmt
g++ -std=c++17 `pkg-config --libs --cflags fmt yaml-cpp` src/main.cpp -o ofbuild -O3
#path+=$PWD
#export PATH
#echo $path
#echo $PATH
