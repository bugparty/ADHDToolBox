#!/bin/bash

# 设置Qt的路径
export PATH=/home/bowman/Qt/6.7.2/gcc_64/bin:$PATH

# 创建并切换到构建目录
mkdir -p build
cd build

# 使用qmake生成Makefile
qmake CONFIG+=release CONFIG-=debug ../AdhdToolBox.pro

# 使用make编译项目
make -f Makefile

# 创建打包目录，复制可执行文件到打包目录
mkdir -p packaging
cp ./analogclock/release/analogclock packaging/analogclock

# 使用linuxdeployqt部署Qt依赖
#https://github.com/probonopd/linuxdeployqt
linuxdeployqt packaging/analogclock

cd packaging
