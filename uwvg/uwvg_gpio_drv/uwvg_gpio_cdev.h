#ifndef _UWVG_GPIO_CDEV_H
#define _UWVG_GPIO_CDEV_H




void gpio_class_create_device(int minor,const char *str);
void gpio_class_destroy_device(int minor);
extern void gpio_ioctl(int which,bool status);



#endif /* _UWVG_GPIO_CDEV_H */