#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/gpio/machine.h>
#include <linux/gpio.h>

#include "uwvg_gpio_cdev.h"

static int gpios[10];

static int gpio_cnt = 0;


void gpio_ioctl(int which,bool status)
{
	if(which < gpio_cnt)
		gpio_set_value(gpios[which],status);
	else 
		printk(KERN_ERR"uwvg_gpio%d is not valid!",which);
}

EXPORT_SYMBOL(gpio_ioctl);

static int driver_probe(struct platform_device *pdev)
{
	int ret;
	int i;
	enum of_gpio_flags flag;
	struct device_node * gpio_node = pdev->dev.of_node;
	struct device_node * child;
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	for_each_child_of_node(gpio_node, child)
    {
        printk(KERN_ERR"child->name=%s,child->full_name=%s,%p\n",child->name,child->full_name,child);
		gpios[gpio_cnt] = of_get_named_gpio_flags(child,"gpios",0,&flag);
		if(!gpio_is_valid(gpios[gpio_cnt]))
		{
			printk(KERN_INFO "hello: invalid gpio : %d\n",gpios[gpio_cnt]);
            goto error;
		}
		else 
			printk(KERN_ERR"gpio is %d",gpios[gpio_cnt]);

		if(flag == GPIO_ACTIVE_HIGH)
			printk(KERN_ERR"YES\n");
		ret = gpio_request(gpios[gpio_cnt],"uwvg-gpio");
		if(ret)
		{
			printk(KERN_ERR"child->name=%s,child->full_name=%s request gpio failed!",child->name,child->full_name);
			goto error;
		}
		gpio_direction_output(gpios[gpio_cnt],flag == GPIO_ACTIVE_HIGH?0:1);
		gpio_class_create_device(gpio_cnt,(const char*)child->name);
		gpio_cnt ++;
	}
	
	return 0;
error:
	for(gpio_cnt--;gpio_cnt >=0;gpio_cnt-- )
	{
		gpio_free(gpios[gpio_cnt]);
		gpio_class_destroy_device(gpio_cnt);
	}
	return -1;
}
static int driver_remove(struct platform_device *pdev)
{
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	for(gpio_cnt--;gpio_cnt >=0;gpio_cnt-- )
	{
		gpio_free(gpios[gpio_cnt]);
		gpio_class_destroy_device(gpio_cnt);
	}
	return 0;
}


static struct of_device_id rk3399_match_table[] = {
	{ .compatible = "uwvg,rk3399-gpio",},
	{},
};

static struct platform_driver driver = {
	.probe = driver_probe,
	.remove = driver_remove,
	.driver = {
		.owner = THIS_MODULE,
		.name = "uwvg-gpio",
		.of_match_table = rk3399_match_table,
	},
	
};


static void __exit rk3399_gpio_driver_exit(void)
{
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	platform_driver_unregister(&driver);

}

static int __init rk3399_gpio_driver_init(void)
{
	printk(KERN_EMERG"%s %s line %d\n", __FILE__, __FUNCTION__, __LINE__);
	platform_driver_register(&driver);
	return 0;
}

module_init(rk3399_gpio_driver_init);
module_exit(rk3399_gpio_driver_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DevoutPrayer<hexiao@hrbeu.edu.cn>");

