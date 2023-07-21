#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/cdev.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ibrahim");
MODULE_DESCRIPTION("A hello world Psuedo device driver");

#define SIZE 255
static unsigned char buffer[SIZE] = "This message from the driver buffer";

int driver_open(struct inode *, struct file *)
{
    printk("Device opened \n");
    return 0;
}

int driver_close(struct inode *, struct file *)
{
    printk("Device closed \n");
    return 0;
}

ssize_t driver_read(struct file *file, char __user *use_buffer, size_t count, loff_t *off)
{
    int not_copied = 0;
    printk("Driver read is called \n");
    printk("Count is %ld \n", count);
    printk("offset is %lld \n", off);
    printk("Driver message %s \n", buffer);
    if (count + *off > SIZE)
    {
        count = SIZE - *off;
    }
    not_copied = copy_to_user(use_buffer, &buffer[*off], count);
    if (not_copied)
    {
        return -1;
    }
    *off = count;
    return 0;
}

struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = driver_open,
    .release = driver_close,
    .read = driver_read};

dev_t dev_number;
struct cdev st_character_dev;
struct class *device_class;
struct device *my_device;

static int __init hello_world_driver_init(void)
{
    int retval = 0;
    printk("Hello Kernel\n");
    // Allocating device number.
    retval = alloc_chrdev_region(&dev_number, 0, 1, "test_device");
    if (retval == 0)
    {
        printk("Registered character device Major %d Minor %d from %s", MAJOR(dev_number), MINOR(dev_number), __FUNCTION__);
    }
    else
    {
        printk("Failed registering character device Major %d Minor %d from %s", MAJOR(dev_number), MINOR(dev_number), __FUNCTION__);
        return -1;
    }
    // Define driver type
    cdev_init(&st_character_dev, &fops);
    retval = cdev_add(&st_character_dev, dev_number, 1);
    if (retval != 0)
    {
        unregister_chrdev_region(dev_number, 1);
        printk("Failed registering device to the kernel %s", __FUNCTION__);
        return -1;
    }

    // Generate the driver file
    if ((device_class = class_create(THIS_MODULE, "test_class")) == NULL)
    {
        printk("Failed creating a class for the device \n");
        cdev_del(&st_character_dev);
        unregister_chrdev_region(dev_number, 1);

        return -1;
    }
    my_device = device_create(device_class, NULL, dev_number, NULL, "test_file");
    if (my_device == NULL)
    {
        printk("Failed creating the device \n");
        cdev_del(&st_character_dev);
        class_destroy(device_class);
        unregister_chrdev_region(dev_number, 1);
        return -1;
    }
    printk("Device driver created\n");
    return 0;
}

static void __exit hello_world_driver_exit(void)
{
    cdev_del(&st_character_dev);
    device_destroy(device_class, dev_number);
    class_destroy(device_class);
    unregister_chrdev_region(dev_number, 1);
    printk("Goodbye World!\n");
}

module_init(hello_world_driver_init);
module_exit(hello_world_driver_exit);