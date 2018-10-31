#!/bin/bash
mkdir -p bin
mkdir -p obj
mkdir -p obj/Release
make $1 --directory=./obj/Release --makefile=../../Makefile build_flags="-O2 -fomit-frame-pointer" program_name=../../bin/application link_flags="-s -pipe -std=c++17"