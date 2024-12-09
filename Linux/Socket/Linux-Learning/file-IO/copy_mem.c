// 通过内存映射的方式拷贝数据
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/mman.h>

int main(int argc, char **argv){
	int fd_old, fd_new;
	struct stat stat;
	char *buf;
	if(argc != 3){
		printf("Usage : %s  <old file>  <new file> \n",argv[0] );
		return -1;
	}
	// 打开旧文件
	fd_old = open(argv[1], O_RDONLY);
	if(fd_old == -1){
		printf("open %s failed !\n", argv[1]);
		return -1;
	}
	// 确定旧文件的大小
	if(fstat(fd_old, &stat) == -1){
		printf("can not get size of %s \n", argv[1]);
		return -1;
	}
	// 映射旧文件
	buf = mmap(NULL, stat.st_size, PROT_READ, MAP_SHARED, fd_old, 0);
	if(buf == MAP_FAILED){
		printf("%s file mmap failed \n", argv[1]);
		return -1;
	}

	// 创建新文件
	fd_new = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	if(fd_new == -1){
		printf("creat new_file %s \n", argv[2]);
		return -1;
	}
	// 向新文件写入数据
	if(write(fd_new, buf, stat.st_size) == -1){
		printf("%s write failed \n ", argv[2]);
		return -1;
	}

	close(fd_old);
	close(fd_new);
	return 0;
}
