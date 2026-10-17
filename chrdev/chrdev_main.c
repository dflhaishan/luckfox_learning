#include "fcntl.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"

static char usrdata[] = { "user data!" };

int main(int argc, char* argv[])
{
    int fd = 0;
    char readbuf[100];
    char writebuf[100];
    int ret = 0;

    fd = open("/dev/chrdev", O_RDWR);
    if (fd < 0) {
        printf("open chrdev failed!\r\n");
        return -1;
    } else {
        printf("open chrdev success!\r\n");
    }

    memset(readbuf, 0, sizeof(readbuf));
    ret = read(fd, readbuf, sizeof(readbuf));
    if (ret < 0) {
        printf("read data from kernel failed!\r\n");
        return -1;
    }
    printf("read data from kernel: %s\r\n", readbuf);

    memset(writebuf, 0, sizeof(writebuf));
    memcpy(writebuf, usrdata, sizeof(usrdata));
    ret = write(fd, writebuf, sizeof(writebuf));
    if (ret < 0) {
        printf("write data to kernel failed!\r\n");
        return -1;
    }
    printf("write data to kernel: %s\r\n", writebuf);

    close(fd);
    return 0;
}