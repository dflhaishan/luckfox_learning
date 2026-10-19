#include "fcntl.h"
#include "linux/fb.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/ioctl.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"

#define OLED_DEV_PATH "/dev/fb0"

int main(int argc, char* argv[])
{
    int oled_fd = 0;
    int ret = 0;

    oled_fd = open(OLED_DEV_PATH, O_RDWR);
    if (oled_fd < 0) {
        printf("open file %s failed!\r\n", OLED_DEV_PATH);
        return -1;
    }

    struct fb_fix_screeninfo info = { 0 };

    ret = ioctl(oled_fd, FBIOGET_FSCREENINFO, &info);
    if (ret < 0) {
        printf("ioctl FBIOGET_FSCREENINFO failed %d\r\n", ret);
        close(oled_fd);
        return -1;
    }

    struct fb_var_screeninfo var = { 0 };

    ret = ioctl(oled_fd, FBIOGET_VSCREENINFO, &var);
    if (ret < 0) {
        printf("ioctl FBIOGET_VSCREENINFO failed %d\r\n", ret);
        close(oled_fd);
        return -1;
    }

    printf("xres %d, yres %d, bit_per_pixel %d\r\n",
        var.xres, var.yres, var.bits_per_pixel);

    int mem_len = info.line_length * var.yres_virtual;
    unsigned char* mem = NULL;
    if (mem_len) {
        mem = mmap(NULL, mem_len, PROT_READ | PROT_WRITE,
            MAP_SHARED, oled_fd, 0);
        if (mem == MAP_FAILED) {
            printf("mmap failed %d\r\n", MAP_FAILED);
            close(oled_fd);
            return -1;
        }
    }

    memset(mem, 0x00, mem_len);
    memset(mem, 0xf0, mem_len);

    munmap(mem, mem_len);
    close(oled_fd);
    return 0;
}