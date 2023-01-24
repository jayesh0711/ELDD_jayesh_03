#include<linux/init.h>             
#include<linux/kernel.h>           
#include<linux/module.h>            
#include<linux/kdev_t.h>          
#include<linux/types.h>            
#include<linux/fs.h>                
#include<linux/cdev.h>             
#include<linux/device.h>
#include<linux/uaccess.h>           
#include<linux/slab.h>              
#include<linux/ioctl.h>
#include<linux/timer.h> 
#include<linux/jiffies.h>

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Jayesh Maurya");
MODULE_DESCRIPTION("Led operation using gpio");

#define WR_VALUE _IOW('a','a',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)
#define TIMEOUT 1000

#define NAME RW_CharDriver
#define MEM_SIZE 50

uint8_t *kernel_buffer;                
int32_t value = 0;  


dev_t mydev = 0;                        
static struct class *dev_class;         
static struct cdev my_cdev;           

static int      __init charDev_fops_init(void);
static void     __exit charDev_fops_exit(void);
static struct timer_list hc_timer;
static int      char_open(struct inode *pinode, struct file *pfile);
static int      char_release(struct inode *pinode, struct file *pfile);
static ssize_t  char_read(struct file *filp, char __user *Ubuf, size_t len,loff_t * offp);
static ssize_t  char_write(struct file *filp, const char *Ubuf, size_t len, loff_t * offp);
static long     char_ioctl(struct file *file, unsigned int cmd, unsigned long arg);



struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .read           = char_read,
    .write          = char_write,
    .open           = char_open,
    .unlocked_ioctl = char_ioctl,
    .release        = char_release,

};

static int char_open(struct inode *pinode, struct file *pfile) 
{
    pr_alert("Driver Open function called\n");
    printk(KERN_INFO "Device file opened\n");
    return 0;
}

static int char_release(struct inode *pinode, struct file *pfile) 
{
    pr_alert("Driver Release function called\n");
    printk(KERN_INFO "Device File Closed\n");
    
    return 0;
} 
void timer_callback(struct timer_list * data)
{
   
    pr_info("Timer Callback function Called [%d]\n",count++);
    
    
    mod_timer(&hc_timer, jiffies + msecs_to_jiffies(TIMEOUT));
}



static ssize_t char_write(struct file *filp, const char *Ubuf, size_t len, loff_t *offp) 
{
    printk("write function called");
    
}
static ssize_t char_read(struct file *filp, char __user *Ubuf, size_t len, loff_t *offp) 
{buff
    printk(KERN_ALERT "This is the kernel read call...Inside %s call\n", __FUNCTION__);
    pr_info("Driver Read function called...........!!!\n");

    return 0;
}



static int __init charDev_fops_init(void) 
{
    printk(KERN_ALERT "Inside %s Module\n", __FUNCTION__);
    printk(KERN_INFO "Inserting Device Driver into Kernel\n");
    
    if(alloc_chrdev_region(&mydev,0,1,"charDev_fops") < 0) {
        pr_err("Cannot allocate Major number\n");
        return -1;
    }
    pr_info("Device Number: Major = %d Minor = %d\n", MAJOR(mydev), MINOR(mydev));


    cdev_init(&my_cdev, &fops);

    if((cdev_add(&my_cdev,mydev,1)) < 0) {
        pr_err("Cannot add device to the system\n");
        goto r_class;
    }

    if((dev_class = class_create(THIS_MODULE,"charDev_class")) == NULL) {
        printk(KERN_ERR "Cannot create the struct class for device\n");
        goto r_class;
    }

    if((device_create(dev_class,NULL,mydev,NULL,"charDev_device")) == NULL) {
        pr_err("Cannot create the Device\n");
        goto r_device;
    }

    timer_setup(&hc_timer, timer_callback, 0);       
    
    mod_timer(&hc_timer, jiffies + msecs_to_jiffies(TIMEOUT));

    pr_info("Device Driver Inserted... Successfully\n");

    return 0;
r_device:
    class_destroy(dev_class);

r_class:
    unregister_chrdev_region(mydev,1);
    return -1;

}


static void __exit charDev_fops_exit(void) 
{
    printk(KERN_ALERT "Inside %s Module\n", __FUNCTION__);
    printk(KERN_INFO "Removing charDev_fops from Kernel\n");
    device_destroy(dev_class,mydev);                // To destroy char device 
    class_destroy(dev_class);                       // To destroy device class
    cdev_del(&my_cdev);                         
    unregister_chrdev_region(mydev,1);               
    pr_info("Device Driver removed... Successfully\n");
}


module_init(charDev_fops_init);        
module_exit(charDev_fops_exit);      


