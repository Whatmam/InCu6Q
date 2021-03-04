#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/of_gpio.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/workqueue.h>

#define DEVICE_NAME "gpio_switch"

#define             IOCTL_MAGIC         200
#define             SET_DATA            _IOW(IOCTL_MAGIC, 2 ,struct ioctl_info)
#define             GET_DATA            _IOR(IOCTL_MAGIC, 3 ,struct ioctl_info)

#define             GPIO2_08            40

struct gpio_data
{
        unsigned int number; 
        unsigned int value;
};

struct ioctl_info{
       unsigned long size;
       char buf[128];
};

struct gpio_data gpio_buf_data;
static struct ioctl_info info;


ssize_t gpio_switch_read(struct file *f, char __user *data, size_t size, loff_t *fops)
{
    return 0;
}

long gpio_switch_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    char state[10];

    switch(cmd)
    {
        case GPIO2_08:
            arg == 1 ? strcpy(state,"HIGH") : strcpy(state,"LOW");

            printk("[DEV] gpio_direction_output GPIO2_08 %s\r\n",state);
            gpio_direction_output(40, arg);
            break;
        
        case SET_DATA:
            //printk("[DEV] SET_DATA\r\n");
            if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
                return -EFAULT;
            }
            printk("[DEV] info.size : %ld, info.buf : %s",info.size, info.buf);
            break;

        case GET_DATA:
           // printk("[DEV] GET_DATA\r\n");
            if (copy_to_user((void __user *)arg, &info, sizeof(info))) {
                return -EFAULT;
            }
            break;

        default:
            printk("[DEV] default\r\n");
            break;
    }  

    return 0;
}

ssize_t gpio_switch_write(struct file *f, const char __user *data, size_t size, loff_t *fops)
{
    int err = copy_from_user(&gpio_buf_data, data, sizeof(gpio_buf_data));

    if(err < 0)
    {
        printk("[DEV] gpio_switch : data copy error!!\r\n");
        return -1;
    }
    else{
        printk("[DEV] number : %d, IO : %d\r\n", gpio_buf_data.number, gpio_buf_data.value);
        //gpio_request(gpio, "gpio_switch");
        //gpio_set_value(gpio_buf_data.number,gpio_buf_data.value);
        //gpio_direction_output(gpio_buf_data.number, gpio_buf_data.value);
    }

    return 0;
}

static int gpio_switch_open(struct inode *i, struct file *f)
{
    return 0;
}

static struct file_operations gpio_switch_fops = {
    .read = gpio_switch_read,
    .write = gpio_switch_write,
    .open = gpio_switch_open,
    .unlocked_ioctl = gpio_switch_ioctl
};

static int gpio_switch_probe(struct platform_device *pdev)
{
    return 0;
}

static int gpio_switch_remove(struct platform_device *pdev)
{
    return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id gpio_switch_match[] = {
    {   .compatible = "gpio_switch", },
    { },
};
#endif

static struct platform_driver gpio_switch_driver = {
    .probe = gpio_switch_probe,
    .remove = gpio_switch_remove,
    .driver = {
        .name = "gpio_switch",
        .of_match_table = of_match_ptr(gpio_switch_match),
    },
};
module_platform_driver(gpio_switch_driver);

int __init gpio_switch_device_init(void)
{
    int ret = 0;
    struct class *class;
    struct device *dev;

    ret = platform_driver_register(&gpio_switch_driver);
            
    if (ret < 0)
        printk("platform driver register FAIL ret=%d\r\n", ret);

    ret = register_chrdev(200, DEVICE_NAME, &gpio_switch_fops);

    printk("%s\n", __func__);
    if (ret < 0)
        printk("register character device fail\r\n");

     class = class_create(THIS_MODULE, DEVICE_NAME);
     dev = device_create(class, NULL, MKDEV(200, 0), NULL, DEVICE_NAME);

    return 0;
}

void __exit gpio_switch_device_exit(void)
{
    platform_driver_unregister(&gpio_switch_driver);
}

module_init(gpio_switch_device_init);
module_exit(gpio_switch_device_exit);

