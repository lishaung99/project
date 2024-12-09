SRC = $(wildcard ./src/*.c)  #获取所有的.c文件  
#特殊项目编号,MIPS-CC = mipsel-openwrt-linux-gcc
MIPS-CC = gcc

CFLAGS += -DDB_PATH='"../chatRome.db"'

OBJ = server
TOPDIR	:= $(CURDIR)

INCLUDES = -I./include 
#LIB_DIR	:= -lpthread -lcjson -luci
LIB_DIR += -L$(CURDIR)/lib -lpthread -lsqlite3
# 默认目标,每次运行 make 时，都会先执行 clean 规则，然后再编译生成 server 文件。
all: clean $(OBJ) 
#编译
$(OBJ): $(SRC)
	$(MIPS-CC) $(CFLAGS) -o $@ $^ $(INCLUDES) $(LIB_DIR)



clean:
	rm -rf *~ *.o server
