#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>

#define CHRDEV_CNT 1
#define CHRDEV_NAME "chrdev"

struct chrdev_t {
    dev_t devid;
    struct cdev cdev;
    struct class* class;
    struct device* device;
    int major;
    int minor;
} chrdev;

static char readbuf[100];
static char writebuf[100];

static char kerneldata[] = { "kernel data!" };

static int chrdev_open(struct inode* inode, struct file* filp)
{
    filp->private_data = &chrdev;
    printk("chrdev open!\r\n");
    return 0;
}

static int chrdev_release(struct inode* inode, struct file* filp)
{
    printk("chrdev release!\r\n");
    return 0;
}

static ssize_t chrdev_read(struct file* filp, char __user* buf, size_t cnt,
    loff_t* offt)
{
    int ret = 0;
    memcpy(readbuf, kerneldata, sizeof(kerneldata));
    ret = copy_to_user(buf, readbuf, cnt);
    if (ret == 0) {
        printk("kernel send data to user!\r\n");
    } else {
        printk("kernel send data to user failed!\r\n");
    }
    return 0;
}

static ssize_t chrdev_write(struct file* filp, const char __user* buf,
    size_t cnt, loff_t* offt)
{
    int ret = 0;
    ret = copy_from_user(writebuf, buf, cnt);
    if (ret == 0) {
        printk("kernel receive data from user!\r\n");
    } else {
        printk("kernel receive data from user failed!\r\n");
    }
    return 0;
}

static struct file_operations chrdev_fops = {
    .owner = THIS_MODULE,
    .open = chrdev_open,
    .release = chrdev_release,
    .read = chrdev_read,
    .write = chrdev_write,
};

static int __init chrdev_init(void)
{
    if (chrdev.major) {
        chrdev.devid = MKDEV(chrdev.major, 0);
        register_chrdev_region(chrdev.devid, CHRDEV_CNT, CHRDEV_NAME);
    } else {
        alloc_chrdev_region(&chrdev.devid, 0, CHRDEV_CNT, CHRDEV_NAME);
        chrdev.major = MAJOR(chrdev.devid);
        chrdev.minor = MINOR(chrdev.devid);
    }
    printk("chrdev major = %d, minor = %d\r\n", chrdev.major, chrdev.minor);

    chrdev.cdev.owner = THIS_MODULE;
    cdev_init(&chrdev.cdev, &chrdev_fops);
    cdev_add(&chrdev.cdev, chrdev.devid, CHRDEV_CNT);
    chrdev.class = class_create(THIS_MODULE, CHRDEV_NAME);
    if (IS_ERR(chrdev.class)) {
        return PTR_ERR(chrdev.class);
    }
    chrdev.device = device_create(chrdev.class, NULL, chrdev.devid, NULL, CHRDEV_NAME);
    if (IS_ERR(chrdev.device)) {
        return PTR_ERR(chrdev.device);
    }
    return 0;
}

static void __exit chrdev_exit(void)
{
    device_destroy(chrdev.class, chrdev.devid);
    class_destroy(chrdev.class);
    cdev_del(&chrdev.cdev);
    unregister_chrdev_region(chrdev.devid, CHRDEV_CNT);
    printk("chrdev_exit!\r\n");
}

module_init(chrdev_init);
module_exit(chrdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("luckfox");