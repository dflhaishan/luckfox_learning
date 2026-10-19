#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#define MISC_TEST_MINOR 200
#define MISC_TEST_NAME "haishan"

struct test_dev_t {
    dev_t devid;
    struct cdev cdev;
    struct class* class;
    struct device* device;
} test_dev;

static int test_open(struct inode* inode, struct file* filp)
{
    filp->private_data = &test_dev;
    printk("test_driver_open\r\n");
    return 0;
}

static ssize_t test_write(struct file* filp, const char __user* buf, size_t count, loff_t* offt)
{
    printk("test_driver_write\r\n");
    return 0;
}

static ssize_t test_read(struct file* filp, char __user* buf, size_t count, loff_t* offt)
{
    printk("test_driver_read\r\n");
    return 0;
}

static struct file_operations test_fops = {
    .owner = THIS_MODULE,
    .open = test_open,
    .write = test_write,
    .read = test_read,
};

static struct miscdevice test_miscdev = {
    .minor = MISC_TEST_MINOR,
    .name = MISC_TEST_NAME,
    .fops = &test_fops,
};

static struct of_device_id test_of_match[]
    = {
          { .compatible = "jhs-haishan" },
          {},
      };

static int test_probe(struct platform_device* pdev)
{
    int ret = 0;
    const char* label;
    printk("test driver and device is matched\r\n");

    if (pdev->dev.of_node != NULL) {
        ret = of_property_read_string(pdev->dev.of_node, "label", &label);
        if (ret) {
            printk("label property read failed\r\n");
        } else {
            printk("label: %s\r\n", label);
        }
    } else {
        printk("test driver and device is not matched\r\n");
        return -EINVAL;
    }

    ret = misc_register(&test_miscdev);
    if (ret) {
        printk("failed to register misc device\r\n");
        return -EFAULT;
    }

    return 0;
}

static int test_remove(struct platform_device* pdev)
{
    printk("test_driver_remove\r\n");

    misc_deregister(&test_miscdev);
    return 0;
}

static struct platform_driver test_driver = {
    .driver = {
        .name = "test_driver",
        .of_match_table = test_of_match,
    },
    .probe = test_probe,
    .remove = test_remove,
};

static int __init test_init(void)
{
    printk("test_driver_init\r\n");

    return platform_driver_register(&test_driver);
}

static void __exit test_exit(void)
{
    printk("test_driver_exit\r\n");
    platform_driver_unregister(&test_driver);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("luckfox");