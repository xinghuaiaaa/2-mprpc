#!/bin/bash

set -e  # 如果有错误，立即退出

rm -rf `pwd`/build/*  # 删除旧的构建目录   反引号
cd `pwd`/build  # 进入构建目录
    cmake .. &&
    make -j4 

cd ..
mv -r `pwd`/src/include `pwd`/lib   # 复制头文件到lib目录  lib里还有一个静态库
