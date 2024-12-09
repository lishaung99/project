# SRC = $(wildcard ./src/*.c)  #获取所有的.c文件  
SRC = $(shell ls ./src/*.c)  #获取所有的.c文件  
#特殊项目编号,MIPS-CC = mipsel-openwrt-linux-gcc
MIPS-CC = gcc

OBJ = hello
TOPDIR	:= $(CURDIR)
# CURDIR 表示当前目录

INCLUDES = -I./include 
#LIB_DIR	:= -lpthread -lcjson -luci

# 默认目标,每次运行 make 时，都会先执行 clean 规则，然后再编译生成 hello 文件。
all: clean $(OBJ) 
#编译
$(OBJ): $(SRC)
	$(MIPS-CC) -o $@ $^ $(INCLUDES) 



clean:
	rm -rf *~ *.o hello
