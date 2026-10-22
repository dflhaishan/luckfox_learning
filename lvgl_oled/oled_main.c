#include "fcntl.h"
#include "linux/fb.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "sys/ioctl.h"
#include "sys/mman.h"
#include "sys/stat.h"
#include "sys/types.h"
#include "time.h"
#include "unistd.h"

#define OLED_DEV_PATH "/dev/fb0"

static unsigned char* fb_mem = NULL;

struct fb_fix_screeninfo info;

struct fb_var_screeninfo var;

static uint32_t tick_get_cb(void)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

static void fbdev_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
{
    px_map += 8; // 跳过调色板，必须！

    int32_t w_bytes = lv_area_get_width(area) / 8; // I1 下宽度按字节对齐
    int32_t h = lv_area_get_height(area);

    for (int32_t y = 0; y < h; y++) {
        uint8_t* dst = fb_mem
            + (area->y1 + y) * info.line_length
            + area->x1 / 8;
        memcpy(dst, px_map + y * w_bytes, w_bytes);
    }

    lv_display_flush_ready(disp);
}

int main(int argc, char* argv[])
{
    int oled_fd = 0;
    int ret = 0;

    oled_fd = open(OLED_DEV_PATH, O_RDWR);
    if (oled_fd < 0) {
        printf("open file %s failed!\r\n", OLED_DEV_PATH);
        return -1;
    }

    ret = ioctl(oled_fd, FBIOGET_FSCREENINFO, &info);
    if (ret < 0) {
        printf("ioctl FBIOGET_FSCREENINFO failed %d\r\n", ret);
        close(oled_fd);
        return -1;
    }

    ret = ioctl(oled_fd, FBIOGET_VSCREENINFO, &var);
    if (ret < 0) {
        printf("ioctl FBIOGET_VSCREENINFO failed %d\r\n", ret);
        close(oled_fd);
        return -1;
    }

    printf("xres %d, yres %d, bit_per_pixel %d\r\n",
        var.xres, var.yres, var.bits_per_pixel);

    int mem_len = info.line_length * var.yres_virtual;
    if (mem_len) {
        fb_mem = mmap(NULL, mem_len, PROT_READ | PROT_WRITE,
            MAP_SHARED, oled_fd, 0);
        if (fb_mem == MAP_FAILED) {
            printf("mmap failed\r\n");
            close(oled_fd);
            return -1;
        }
    }

    memset(fb_mem, 0x00, mem_len);
#if 1
    lv_init();
    lv_tick_set_cb(tick_get_cb);
    lv_display_t* display = lv_display_create(var.xres, var.yres);

    size_t buf_px = var.xres * var.yres;
    size_t buf_bytes = buf_px * sizeof(lv_color_t);
    void* lv_buf = malloc(buf_bytes);
    if (!lv_buf) {
        printf("lv_buf malloc failed\r\n");
        close(oled_fd);
        return -1;
    }

    lv_display_set_buffers(display, lv_buf, NULL, buf_bytes, LV_DISPLAY_RENDER_MODE_DIRECT);
    lv_display_set_flush_cb(display, fbdev_flush);

    if (var.bits_per_pixel == 8) {
        lv_display_set_color_format(display, LV_COLOR_FORMAT_I1);
    }

    lv_obj_t* label1 = lv_label_create(lv_screen_active());
    lv_label_set_text(label1, "hello world.");

    while (1) {
        uint32_t idle = lv_timer_handler();
        if (idle == LV_NO_TIMER_READY) {
            idle = 100;
        }
        usleep(idle * 1000);
    }
    free(lv_buf);
#endif
    munmap(fb_mem, mem_len);
    close(oled_fd);

    return 0;
}