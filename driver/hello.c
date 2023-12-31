#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/module.h>
#include <linux/moduleparam.h>

/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ibrahim");
MODULE_DESCRIPTION("A hello world Psuedo device driver");

#define SIZE 16
#define GPIO_PIN 2
static unsigned char buffer[SIZE] = "";

int driver_open(struct inode *node, struct file *file) {
  printk("Device opened \n");
  return 0;
}

int driver_close(struct inode *node, struct file *file) {
  printk("Device closed \n");
  return 0;
}

ssize_t driver_read(struct file *file, char __user *use_buffer, size_t count,
                    loff_t *off) {
  int not_copied = 0;
  printk("Driver read is called \n");
  printk("Count is %ld \n", count);
  printk("offset is %lld \n", *off);
  printk("Driver message %s \n", buffer);
  if (count + *off > SIZE) {
    count = SIZE - *off;
  }
  not_copied = copy_to_user(use_buffer, &buffer[*off], count);
  if (not_copied) {
    return -1;
  }
  *off = count;
  return count;
}

ssize_t driver_write(struct file *file, const char __user *user_buffer,
                     size_t count, loff_t *off) {
  int not_copied = 0;
  printk("Driver write is called \n");
  printk("Count to write %ld \n", count);
  printk("offset is %lld \n", *off);
  if (count + *off > SIZE) {
    count = SIZE - *off;
  }
  if (!count) {
    printk("No space left to write \n");
    return -1;
  }
  not_copied = copy_from_user(&buffer[*off], user_buffer, count);
  switch (buffer[0]) {
  case '1':
    printk("Switching on the LED \n");
    gpio_set_value(GPIO_PIN, 1);
    break;
  case '0':
    printk("Switching OFF the LED \n");
    gpio_set_value(GPIO_PIN, 0);
    break;
  }
  if (not_copied) {
    return -1;
  }
  *off = count;
  printk("Driver message %s \n", buffer);
  return count;
}

struct file_operations fops = {.owner = THIS_MODULE,
                               .open = driver_open,
                               .release = driver_close,
                               .read = driver_read,
                               .write = driver_write};

dev_t dev_number;
struct cdev st_character_dev;
struct class *device_class;
struct device *my_device;

static int __init hello_world_driver_init(void) {
  int retval = 0;
  printk("Hello Kernel\n");
  // Allocating device number.
  retval = alloc_chrdev_region(&dev_number, 0, 1, "test_device");
  if (retval == 0) {
    printk("Registered character device Major %d Minor %d from %s",
           MAJOR(dev_number), MINOR(dev_number), __FUNCTION__);
  } else {
    printk("Failed registering character device Major %d Minor %d from %s",
           MAJOR(dev_number), MINOR(dev_number), __FUNCTION__);
    return -1;
  }
  // Define driver type
  cdev_init(&st_character_dev, &fops);
  retval = cdev_add(&st_character_dev, dev_number, 1);
  if (retval != 0) {
    unregister_chrdev_region(dev_number, 1);
    printk("Failed registering device to the kernel %s", __FUNCTION__);
    return -1;
  }

  // Generate the driver file
  if ((device_class = class_create(THIS_MODULE, "test_class")) == NULL) {
    printk("Failed creating a class for the device \n");
    cdev_del(&st_character_dev);
    unregister_chrdev_region(dev_number, 1);

    return -1;
  }
  my_device = device_create(device_class, NULL, dev_number, NULL, "hello_dev");
  if (my_device == NULL) {
    printk("Failed creating the device \n");
    cdev_del(&st_character_dev);
    class_destroy(device_class);
    unregister_chrdev_region(dev_number, 1);
    return -1;
  }
  if (gpio_request(GPIO_PIN, "gpio_led_2")) {
    printk("Error allocating GPIO %d\n", GPIO_PIN);
    cdev_del(&st_character_dev);
    class_destroy(device_class);
    unregister_chrdev_region(dev_number, 1);
    return -1;
  }
  if (gpio_direction_output(GPIO_PIN, 0)) {
    printk("Error setting GPIO pin to output\n");
    gpio_free(GPIO_PIN);
    cdev_del(&st_character_dev);
    class_destroy(device_class);
    unregister_chrdev_region(dev_number, 1);
    return -1;
  }
  printk("Device driver created\n");
  return 0;
}

static void __exit hello_world_driver_exit(void) {
  cdev_del(&st_character_dev);
  device_destroy(device_class, dev_number);
  class_destroy(device_class);
  unregister_chrdev_region(dev_number, 1);
  gpio_set_value(GPIO_PIN, 0);
  gpio_free(GPIO_PIN);
  printk("Goodbye World!\n");
}

module_init(hello_world_driver_init);
module_exit(hello_world_driver_exit);