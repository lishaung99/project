#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char **argv){
	// 拷贝文件txt1 txt2
	int fd_old, fd_new;
	char buffer[1024];
	int copy_len;

    // 判断命令参数
    if(argc != 3){
        printf("please usage %s <old_file> <new_file>\n", argv[0]);
        return -1;
    }
	//打开文件
	fd_old = open(argv[1], O_RDONLY);
	if(fd_old == -1){
		printf("open filed!");
		return -1;
	}

	// 创建新文件
	fd_new = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	if(fd_new == -1){
		printf("creat new_file %s \n", argv[2]);
		return -1;
	}
	// 循环读取写入
	while((copy_len = read(fd_old, buffer, 1024)) > 0){
		if(write(fd_new, buffer, copy_len) != copy_len){
			printf("can not write %s", argv[2]);
			return -1;
		}

	}	
	// 关闭文件
	close(fd_old);
	close(fd_new);

	return 0;
}
