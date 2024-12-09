SRC = $(wildcard ./src/*.c)  #获取所有的.c文件  
#特殊项目编号,MIPS-CC = mipsel-openwrt-linux-gcc
MIPS-CC = gcc
#CFLAGS := -std=gnu99
#CFLAGS += -fgnu89-inline
#CFLAGS += -DDEBUG	#打印信息
#CFLAGS += -Wall	#打印警告

#CFLAGS +=  -pthread -Ddevice_5350 



#允许打印时间
#CFLAGS += -DPRT_TIME=0

#包含LUA文件
#CFLAGS += -DIncludeLuaFile=1
#CFLAGS += -g -O0
#CFLAGS += -DMEMWATCH -DMW_STDIO
#CFLAGS += -fstack-protector-all
#CFLAGS += -D_FORTIFY_SOURCE=2
# CFLAGS += -DDB_PATH='"../chatRome.db"'

OBJ = server
TOPDIR	:= $(CURDIR)

INCLUDES = -I./include 
#LIB_DIR	:= -lpthread -lcjson -luci
LIB_DIR += -L$(CURDIR)/lib -lpthread 
#编译
$(OBJ): $(SRC)
	$(MIPS-CC) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LIB_DIR)


# 生成lynkros校验
#	../module/CRC/CRC  ../module/CRC/update_cpu_7628 


clean:
	rm -rf *~ *.o server
