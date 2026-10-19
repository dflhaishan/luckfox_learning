#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"

#define DEV_PATH "/dev/haishan"

int main(int argc, char* argv[])
{
    int fd = open(DEV_PATH, O_RDWR);
    int ret = 0;
    if (fd < 0) {
        printf("open file failed\n");
        return -1;
    }
    ret = write(fd, "hello world", strlen("hello world"));
    if (ret < 0) {
        printf("write file failed\n");
        close(fd);
        return -1;
    }

    ret = read(fd, NULL, 0);
    if (ret < 0) {
        printf("read file failed\n");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}