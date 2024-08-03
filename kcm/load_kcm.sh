#!/bin/bash

make clean
make
sudo modprobe -r mykcm
# 定义内核模块目录和模块文件
MODULE_NAME="mykcm.ko"
MODULE_DIR="/lib/modules/$(uname -r)/kernel/drivers/"
MODULE_PATH="$MODULE_DIR$MODULE_NAME"

# 检查是否以 root 用户运行
if [ "$(id -u)" -ne "0" ]; then
  echo "请以 root 用户运行此脚本"
  exit 1
fi

# 拷贝内核模块到内核模块目录
echo "拷贝 $MODULE_NAME 到 $MODULE_DIR"
sudo cp $MODULE_NAME $MODULE_DIR

# 更新模块依赖
echo "更新模块依赖"
sudo depmod -a

# 加载内核模块
echo "加载内核模块 $MODULE_NAME"
sudo modprobe mykcm

# 检查模块是否成功加载
if lsmod | grep "mykcm" &> /dev/null ; then
    echo "模块 $MODULE_NAME 已成功加载"
else
    echo "模块 $MODULE_NAME 加载失败"
fi
sudo chmod 666 /dev/mykcm
