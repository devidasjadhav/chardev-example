#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>  
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/anon_inodes.h>

#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");     
MODULE_AUTHOR("Devidas"); 
MODULE_DESCRIPTION("SysFS get set"); 
MODULE_VERSION("0.1"); 
        struct cdev temp_cdev;
        dev_t temp_dev;
        struct class *temp_class;
        struct device *temp_device[5];


int size_of_message = 11;
char message[1024]= "DummyData\n";
int current_DATA = 0;
//DECLARE_WAIT_QUEUE_HEAD(wait_queue_name);
//wait_queue_head_t my_queue;
//init_waitqueue_head(&my_queue);

int ftest_open(struct inode *inode, struct file *filp){
	printk(KERN_INFO "Open Fopen!!!\n");
	return 0;
}



static ssize_t ftest_read(struct file *filep, char *buffer, size_t len, loff_t *offset){
   int ret = 0;
   printk(KERN_INFO "Read Fopen!!!\n");
   if(size_of_message == 0)
      return 0;
   printk(KERN_INFO "Bye.Read%s\n",filep->f_path.dentry->d_iname);
   ret = copy_to_user(buffer,message,size_of_message);
   ret = size_of_message;
   size_of_message=0;
   return ret;
}

static ssize_t ftest_write(struct file *filep, const char *buffer, size_t len, loff_t *offset){
	printk(KERN_INFO "Write Fopen %s!!!\n",filep->f_path.dentry->d_iname);
        if(copy_from_user(message,buffer,len))
		return -EFAULT;  
        size_of_message = len;
        return len;
}
static int ftest_release(struct inode *inodep, struct file *filep){
   printk(KERN_INFO "EBBChar: Device successfully closed\n");
   return 0;
}
static struct file_operations ftest_fops =
{
   .open = ftest_open,
   .read = ftest_read,
   .write = ftest_write,
   .release = ftest_release,
};


int sysfs_test_set(const char *val, const struct kernel_param *kp){
	printk(KERN_INFO "SET:%s\n",val);
	return 0;
}

char dataBuff[1024];
int sysfs_test_get(char *buffer, const struct kernel_param *kp){
//	strcpy(buffer,"Hello");
	int ret = 0;	
	int fdNo = anon_inode_getfd("test",&ftest_fops, dataBuff ,O_RDWR);
	printk(KERN_INFO "GET:%d\n",fdNo);
	//ret = put_user(fdNo,(int *)buffer);	
	ret = sprintf(buffer ,"%d",fdNo);
	printk(KERN_INFO "GET:%d\n",ret);

	return ret;
}


const struct kernel_param_ops sysfs_test_ops = 
{
    .set = &sysfs_test_set, // Use our setter ...
    .get = &sysfs_test_get
};
//unsigned short param = 0xff;
//char param[20]= {0};
//module_param_cb(test,&sysfs_test_ops,&param,S_IRUGO | S_IWUSR);


static int __init hello_init(void)
{
	int i;
    //INIT_LIST_HEAD(&listMember.list);
    alloc_chrdev_region(&temp_dev,0,5,"dummy");
        cdev_init(&temp_cdev,&ftest_fops);
        cdev_add(&temp_cdev,temp_dev,5);
        temp_class = class_create(THIS_MODULE,"test");
	for(i=0;i<5;i++){
        	printk("major %d minor %d\n", MAJOR(temp_dev),MINOR(temp_dev));
//        	temp_device[i] = device_create(temp_class , NULL,temp_dev, "this is private data", "hello%d%d",MAJOR(temp_dev),MINOR(temp_dev));
        	temp_device[i] = device_create(temp_class , NULL,temp_dev, "this is private data", "hello%d",MINOR(temp_dev));
		temp_dev++;
	}

    printk(KERN_INFO "Hello!!!\n");
    return 0;
}

static void __exit hello_cleanup(void)
{
  int i;
  printk(KERN_INFO "Bye.\n");
  for(i=0;i<5;i++){
	temp_dev--;
	device_destroy(temp_class,temp_dev);
  }
  class_destroy(temp_class);
  cdev_del(&temp_cdev);
  unregister_chrdev_region(temp_dev,5);
 
}

module_init(hello_init);
module_exit(hello_cleanup);
