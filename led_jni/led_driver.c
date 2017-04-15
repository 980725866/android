#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define DEVICE_NAME "leds"

static dev_t led_dev;
struct cdev  led_cdev;
static struct class *led_class;
struct device *led_device;

struct led_desc
{
	int gpio;
	char name[10];
};

static struct led_desc led_gpios[] = {
	{EXYNOS4212_GPM4(0),"led1"},
	{EXYNOS4212_GPM4(1),"led2"},
	{EXYNOS4212_GPM4(2),"led3"},
	{EXYNOS4212_GPM4(3),"led4"},
};

static long tiny4412_leds_ioctl(struct file *filp, unsigned int cmd,
		unsigned long args)
{
	switch(cmd)
	{
		case 0:
		case 1:
			if(args>=0 && args<4){
				gpio_set_value(led_gpios[args].gpio,!cmd);
			}
			break;
	}
	return 0;
}

static int tiny4412_leds_open(struct inode *node, struct file *filp)
{
	printk(KERN_INFO "tiny4412_leds_open\n");
	return 0;
}

static int tiny4412_leds_release(struct inode *node, struct file *filp)
{
	printk(KERN_INFO "tiny4412_leds_release\n");
	return 0;
}

static struct file_operations tiny4412_led_fops = 
{
	.owner			= THIS_MODULE,
	.open			= tiny4412_leds_open,
	.release 		= tiny4412_leds_release,
	.unlocked_ioctl	= tiny4412_leds_ioctl,
};

static int __init tiny4412_led_init(void) 
{
	int ret = 0,i;
	ret = alloc_chrdev_region(&led_dev,0,1,DEVICE_NAME);
	if(ret < 0){
		printk(KERN_INFO "alloc_chrdev_region error\n");
		return ret;
	}
	
	cdev_init(&led_cdev, &tiny4412_led_fops);
	led_cdev.owner = THIS_MODULE;
	ret = cdev_add(&led_cdev, led_dev, 1); //add goto 
	if(ret < 0){
		printk(KERN_INFO "cdev_add error\n");
		return ret;
	}
	led_class = class_create(THIS_MODULE,DEVICE_NAME);
	if (IS_ERR(led_class)){
		goto class_create_failed;
	}
	led_device = device_create(led_class, NULL, led_dev, NULL, DEVICE_NAME);
	if (IS_ERR(led_device)) {
		goto device_create_failed;
		//ret = PTR_ERR(led_device);
	}
	for(i=0;i<4;i++){
		ret = gpio_request(led_gpios[i].gpio,led_gpios[i].name);
		s3c_gpio_cfgpin(led_gpios[i].gpio, S3C_GPIO_OUTPUT);
		gpio_set_value(led_gpios[i].gpio,1);
		
		if(ret < 0){
			while(i>=0){
				gpio_free(led_gpios[i].gpio);
				i--;
			}
			return -1;
		}
	}
	printk(KERN_INFO "add led_driver yes\n");
	return 0;
device_create_failed:
	device_destroy(led_class,led_dev);
	cdev_del(&led_cdev);
	return -1;
class_create_failed:
	cdev_del(&led_cdev);
	return -1;	
}

static void __exit tiny4412_led_exit(void) 
{
	int i = 0;
	device_destroy(led_class,led_dev);
	class_destroy(led_class);
	cdev_del(&led_cdev);
	unregister_chrdev_region(led_dev,1);
	for(i=0;i<4;i++){
		gpio_free(led_gpios[i].gpio);
	}
	printk(KERN_INFO "delete led_driver yes\n");
}

module_init(tiny4412_led_init);
module_exit(tiny4412_led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("led jni");


































