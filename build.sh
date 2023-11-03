#!/bin/bash
# 1. 使用 cmake 生成 Makefile 到 build 目录
cmake -B build .
# 2. 切换到 build 目录并使用生成的 Makefile 进行编译
cd build
make