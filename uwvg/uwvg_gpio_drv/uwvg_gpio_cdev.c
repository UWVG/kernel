#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>

#include "uwvg_gpio_cdev.h"

static int major = 0;
static struct class *gpio_class;
static int i = 0;




void gpio_class_create_device(int minor,const char* str)
{
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	device_create(gpio_class, NULL, MKDEV(major, minor), NULL, "%s", str); 
}

void gpio_class_destroy_device(int minor)
{
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	device_destroy(gpio_class, MKDEV(major, minor));
}



EXPORT_SYMBOL(gpio_class_create_device);
EXPORT_SYMBOL(gpio_class_destroy_device);

static int gpio_open (struct inode *inode, struct file *fd)
{
	int minor = iminor(inode);
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	printk(KERN_EMERG"Open device -minor:%d.",minor);
	return 0;
}

static ssize_t gpio_read (struct file *fd, char __user *buf, size_t length, loff_t *offset)
{
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static ssize_t gpio_write (struct file *fd, const char __user *buf, size_t length, loff_t *offset)
{
	struct inode * node = file_inode(fd);
	int minor = iminor(node);
	gpio_ioctl(minor,1);
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static int gpio_release (struct inode *inode, struct file *fd)
{
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	return 0;
}

static long gpio_ioctl_oper (struct file *fd, unsigned int cmd, unsigned long arg) 
{
	struct inode * node = file_inode(fd);
	int minor = iminor(node);
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	if(cmd == 1 )
		gpio_ioctl(minor,true);
	else
	{
		gpio_ioctl(minor,false);
	}

	return 0;
}




static struct file_operations gpio_drv = {
	.owner 	= THIS_MODULE,
	.open 	= gpio_open,
	.read	= gpio_read,
	.write 	= gpio_write,
	.release= gpio_release,
	.unlocked_ioctl = gpio_ioctl_oper,
};


static int __init rk3399_gpio_cdev_init(void)
{
	int ret;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);	
	major = register_chrdev(0,"uwvg_gpio_cdev",&gpio_drv);
	gpio_class = class_create(THIS_MODULE, "uwvg_gpio_class");
	ret = PTR_ERR(gpio_class);
	if(IS_ERR(gpio_class))
	{
		unregister_chrdev(major,"uwvg_gpio_class");
		return -1;
	}
	return 0;
}

static void __exit rk3399_gpio_cdev_exit(void)
{
	int i;
	printk("%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	class_destroy(gpio_class);	
	unregister_chrdev(major,"uwvg_gpio_class");
}




module_init(rk3399_gpio_cdev_init);
module_exit(rk3399_gpio_cdev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DevoutPrayer<hexiao@hrbeu.edu.cn>");

