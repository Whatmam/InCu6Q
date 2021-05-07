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

#define DEVICE_NAME "gpio_jw"

#define             IOCTL_MAGIC         200
#define             SET_DATA            _IOW(IOCTL_MAGIC, 2 ,struct ioctl_info)
#define             GET_DATA            _IOR(IOCTL_MAGIC, 3 ,struct ioctl_info)
#define             SET_LED_CTL_1       _IOW(IOCTL_MAGIC, 110 ,struct ioctl_info)
#define             SET_LED_CTL_2       _IOW(IOCTL_MAGIC, 111 ,struct ioctl_info)
#define             SET_LED_CTL_3       _IOW(IOCTL_MAGIC, 116 ,struct ioctl_info)

#define             GPIO3_16            80
#define             GPIO3_17            81
#define             GPIO3_18            82
#define             GPIO3_19            83
#define             GPIO3_20            84

#define             GPIO5_20            148
#define             GPIO5_21            149
#define             GPIO5_22            150
#define             GPIO5_23            151
#define             GPIO5_24            152
#define             GPIO5_25            153
#define             GPIO5_26            154
#define             GPIO5_27            155

#define             GPIO6_00            160
#define             GPIO6_01            161
#define             GPIO6_02            162
#define             GPIO6_03            163

#define             GPIO7_08            200
#define             GPIO7_12            204

#define             LED_CTL_1            110     //GPIO4_14
#define             LED_CTL_2            111     //GPIO4_15
#define             LED_CTL_3            116     //GPIO4_20

struct gpio_data
{
        unsigned int number; 
        unsigned int value;
};

struct ioctl_info{
        unsigned int number; 
        unsigned int value;
};

struct gpio_data gpio_buf_data;
static struct ioctl_info info;

struct class *class;
struct device *dev;

ssize_t gpio_jw_read(struct file *f, char __user *data, size_t size, loff_t *fops)
{
    return 0;
}

long gpio_jw_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    char state[10];

    switch(cmd)
    {
        case SET_DATA:
            // if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
            //     return -EFAULT;
            // }
            // printk("[DEV] info.size : %ld, info.buf : %s",info.size, info.buf);
            break;

        case GET_DATA:
            if (copy_to_user((void __user *)arg, &info, sizeof(info))) {
                return -EFAULT;
            }
            break;
        case SET_LED_CTL_1:
            if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
                return -EFAULT;
            }

            if(info.value)
            {
                    printk("[DEV] SET_LED_CTL_1 High\r\n");
            }
            else
            {
                    printk("[DEV] SET_LED_CTL_1 Low\r\n");
            }
            gpio_set_value(LED_CTL_1,info.value);
            break;
        case SET_LED_CTL_2:
            if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
                return -EFAULT;
            }
            if(info.value)
            {
                    printk("[DEV] SET_LED_CTL_2 High\r\n");
            }
            else
            {
                    printk("[DEV] SET_LED_CTL_2 Low\r\n");
            }

            gpio_set_value(LED_CTL_2,info.value);
            break;
        case SET_LED_CTL_3:
            if (copy_from_user(&info, (void __user *)arg, sizeof(info))) {
                return -EFAULT;
            }
            if(info.value)
            {
                    printk("[DEV] SET_LED_CTL_3 High\r\n");
            }
            else
            {
                    printk("[DEV] SET_LED_CTL_3 Low\r\n");
            }

            gpio_set_value(LED_CTL_3,info.value);
            break;
        default:
            //Write 
            if(arg == 2 || arg == 3)
            {
                arg == 3 ? strcpy(state,"Set HIGH") : strcpy(state,"Set LOW");
                printk("[DEV] GPIO %s\r\n",state);
                arg &= ~(0x1 << 1);

            gpio_set_value(cmd,arg);
            }
            //Read
            else if(arg ==1)
            {
                info.number = cmd ;
                info.value = gpio_get_value(cmd);

                if (copy_to_user((void __user *)arg, &info, sizeof(info))) {
                return -EFAULT;
                }
            }
            else{
                printk("[DEV] default\r\n");
            }
            break;
    }  

    return 0;
}

ssize_t gpio_jw_write(struct file *f, const char __user *data, size_t size, loff_t *fops)
{
    int err = copy_from_user(&gpio_buf_data, data, sizeof(gpio_buf_data));

    if(err < 0)
    {
        printk("[DEV] gpio_jw : data copy error!!\r\n");
        return -1;
    }
    else{
        printk("[DEV] number : %d, IO : %d\r\n", gpio_buf_data.number, gpio_buf_data.value);
        //gpio_request(gpio, "gpio_jw");
        //gpio_set_value(gpio_buf_data.number,gpio_buf_data.value);
        //gpio_direction_output(gpio_buf_data.number, gpio_buf_data.value);
    }

    return 0;
}

static int gpio_jw_open(struct inode *i, struct file *f)
{
    return 0;
}

static struct file_operations gpio_jw_fops = {
    .read = gpio_jw_read,
    .write = gpio_jw_write,
    .open = gpio_jw_open,
    .unlocked_ioctl = gpio_jw_ioctl
};

static int gpio_jw_probe(struct platform_device *pdev)
{
    int ret;
    /* GPIO_V50_Out 0-3 */
    ret = gpio_request(GPIO3_16, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO3_16, ret);
    else 
        gpio_direction_output(GPIO3_16, 0);
    ret = gpio_request(GPIO3_17, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO3_17, ret);
    else 
        gpio_direction_output(GPIO3_17, 0);
    ret = gpio_request(GPIO3_18, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO3_18, ret);
    else 
        gpio_direction_output(GPIO3_18, 0);
    ret = gpio_request(GPIO3_19, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO3_19, ret);
    else 
        gpio_direction_output(GPIO3_19, 0);
    ret = gpio_request(GPIO3_20, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO3_20, ret);
    else 
        gpio_direction_output(GPIO3_20, 0);

    /* GPIO_V33_In 0-3 */
        ret = gpio_request(GPIO5_20, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_20, ret);
    else 
        gpio_direction_input(GPIO5_20);
        ret = gpio_request(GPIO5_21, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_21, ret);
    else 
        gpio_direction_input(GPIO5_21);
        ret = gpio_request(GPIO5_22, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_22, ret);
    else 
        gpio_direction_input(GPIO5_22);
        ret = gpio_request(GPIO5_23, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_23, ret);
    else 
        gpio_direction_input(GPIO5_23);

    /* GPIO_V33_Out 0-3 */
    ret = gpio_request(GPIO5_24, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_24, ret);
    else 
        gpio_direction_output(GPIO5_24, 0);
    ret = gpio_request(GPIO5_25, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_25, ret);
    else 
        gpio_direction_output(GPIO5_25, 0);
    ret = gpio_request(GPIO5_26, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_26, ret);
    else 
        gpio_direction_output(GPIO5_26, 0);
    ret = gpio_request(GPIO5_27, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO5_27, ret);
    else 
        gpio_direction_output(GPIO5_27, 0);

    /* GPIO_V50_In 0-3*/
        ret = gpio_request(GPIO6_00, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO6_00, ret);
    else 
        gpio_direction_input(GPIO6_00);
        ret = gpio_request(GPIO6_01, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO6_01, ret);
    else 
        gpio_direction_input(GPIO6_01);
        ret = gpio_request(GPIO6_02, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO6_02, ret);
    else 
        gpio_direction_input(GPIO6_02);
        ret = gpio_request(GPIO6_03, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO6_03, ret);
    else 
        gpio_direction_input(GPIO6_03);

    /* GPIO_V50_Out_CTL */ 
    /* GPIO_V50_In_CTL */
        ret = gpio_request(GPIO7_08, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO7_08, ret);
    else 
        gpio_direction_output(GPIO7_08, 0);  

        ret = gpio_request(GPIO7_12, "gpio jw");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", GPIO7_12, ret);
    else 
        gpio_direction_output(GPIO7_12,0);


    /* LED_CTL_1 ~ LED_CTL_3 */
        ret = gpio_request(LED_CTL_1, "LED_CTL_1");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", LED_CTL_1, ret);
    else 
        gpio_direction_output(LED_CTL_1, 0); 

        ret = gpio_request(LED_CTL_2, "LED_CTL_2");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", LED_CTL_2, ret);
    else 
        gpio_direction_output(LED_CTL_2, 0); 

        ret = gpio_request(LED_CTL_3, "LED_CTL_3");
    if(ret)
        printk("#### FAILED Request gpio %d. error : %d \n", LED_CTL_3, ret);
    else 
        gpio_direction_output(LED_CTL_3, 0); 
    return 0;
}

static int gpio_jw_remove(struct platform_device *pdev)
{
    return 0;
}

#ifdef CONFIG_OF
static const struct of_device_id gpio_jw_match[] = {
    {   .compatible = "gpio_jw", },
    { },
};
#endif

static struct platform_driver gpio_jw_driver = {
    .probe = gpio_jw_probe,
    .remove = gpio_jw_remove,
    .driver = {
        .name = "gpio_jw",
        .of_match_table = of_match_ptr(gpio_jw_match),
    },
};
//module_platform_driver(gpio_jw_driver);

static int __init gpio_jw_device_init(void)
{
    int ret = 0;

    printk("%s\n", __func__);
    ret = platform_driver_register(&gpio_jw_driver);
            
    if (ret < 0)
        printk("platform driver register FAIL ret=%d\r\n", ret);

    ret = register_chrdev(200, DEVICE_NAME, &gpio_jw_fops);

    if (ret < 0)
        printk("register character device fail\r\n");

    class = class_create(THIS_MODULE, DEVICE_NAME);
    dev = device_create(class, NULL, MKDEV(200, 0), NULL, DEVICE_NAME);

    return 0;
}

static void __exit gpio_jw_device_exit(void)
{
    int i;
    
    platform_driver_unregister(&gpio_jw_driver);
    unregister_chrdev(200, DEVICE_NAME);
    device_destroy(class,MKDEV(200, 0));
    class_destroy(class);

    for(i=80;i<=84;i++)
        gpio_free(i);
    for(i=148;i<=155;i++)
        gpio_free(i);
    for(i=160;i<=163;i++)
        gpio_free(i);

    gpio_free(110);
    gpio_free(111);
    gpio_free(116);

    gpio_free(200);
    gpio_free(204);
}

module_init(gpio_jw_device_init);
module_exit(gpio_jw_device_exit);

MODULE_LICENSE("GPL");

