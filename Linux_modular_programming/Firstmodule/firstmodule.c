#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h> // to be able to pass parameter to module

#include<linux/fs.h>

#include<linux/cdev.h>
#include<linux/uaccess.h>

//   READING AND WRITING  ***************************
static char buffer[256];
static int buffer_pointer=0;

static ssize_t driver_read(struct file *File, char *user_buffer,size_t count,loff_t *offs){
    int to_copy, not_copied, delta;
    // to_copy= min(count,buffer_pointer);
    if(*offs >= buffer_pointer) return 0;
    to_copy= count<buffer_pointer?count:buffer_pointer;
    not_copied= copy_to_user(user_buffer,buffer,to_copy);
    delta= to_copy - not_copied;
    *offs += delta;
    printk("Read was called");
    return delta;
}

static ssize_t driver_write(struct file *File,const char *user_buffer,size_t count,loff_t *offs){
    int to_copy, not_copied, delta;

    // to_copy= min(count,sizeof(buffer));
    to_copy= count<sizeof(buffer)?count:sizeof(buffer);

    not_copied= copy_from_user(buffer,user_buffer,to_copy);

    buffer_pointer= to_copy;

    delta= to_copy - not_copied;

    printk("Write was called");
    return delta;
}
// **************************************************

//      DEVICE AND DEVICE FILE VARIABLES  ******
// automatically allocating device files and device number (check __init driverInit)
// static dev_t first_device_no;
// static struct class *my_class;
// static struct cdev first_device;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anagh");
MODULE_DESCRIPTION("Hello world module");

// int arg_val=0; // variable to take the argument
// /*
//     registering paramter

//     module_param(name_var,type,permission);

//     permissions: 
//                 S_IRUSR => Integer Reading User
//                 S_IWUSR => Integer Writing User
// */
// module_param(arg_val,int,S_IRUSR | S_IWUSR);

// ARRAY PARAMETER **************
int arg_values[3]={0,0,0};
module_param_array(arg_values,int,NULL,S_IRUSR | S_IWUSR);
// taking values : sudo insmod firstmoudle.ko arg_values=4,3,2
// ******************************

static int driver_open(struct inode *device_file,struct file *instance){
    printk("Open was called\n");
    return 0;
}

static int driver_close(struct inode *device_file,struct file *instance){
    printk("Close was called\n");
    return 0;
}

static struct file_operations fops={
    .owner= THIS_MODULE,
    .open= driver_open,
    .release = driver_close,
    .read= driver_read,
    .write= driver_write
};

#define MYMAJOR 100

// #define DRIVER_NAME "my_driver"
// #define DRIVER_CLASS "MyModuleClass"

static int __init moduleInit(void) {
    printk("Hello from kernel\nArguments passed:\n");
    // for(int i=0;i<3;i++){
    //     printk("%d\n",arg_values[i]);
    //     if(arg_values[i] < 2){
    //         return -1;
    //     }
    // }

    int retval= register_chrdev(MYMAJOR, "first_device_no",&fops); // here we have to manually allocate major number and later define device file manually
    printk("major number:%d minor_numer:%d\n",MYMAJOR,retval);

    // if(alloc_chrdev_region(&first_device_no,0,1,DRIVER_NAME)<0){
    //     printk("Device no. couldn not be allocated");
    //     return -1;
    // }
    // printk("Deviceno. allocated Major:%d minor:%d\n",first_device_no>>20,first_device_no && 0xfffff);

    // // creating device class
    // if((my_class=class_create(DRIVER_CLASS))==NULL){
    //     printk("Class cannot be created\n");
    //     goto ClassError;
    // }
    // // creating device file
    // if(device_create(my_class, NULL, first_device_no,NULL, DRIVER_NAME)==NULL){
    //     printk("Cannot create device file\n");
    //     goto FileError;
    // }
    // // initialize device file
    // cdev_init(&first_device,&fops);

    // // registering device to kernel
    // if(cdev_add(&first_device,first_device_no,-1)==-1){
    //     printk("Registring of device to kernel failed\n");
    //     goto AddError;
    // }
    
    return 0;
    // AddError:
    //     device_destroy(my_class,first_device_no);
    // FileError:
    //     class_destroy(my_class);
    // ClassError:
    //     unregister_chrdev(first_device_no,DRIVER_NAME);
    //     return -1;
}

/**
 * static dev_t first_device_no;
static struct class *my_class;
static struct cdev first_device;
 */

static void __exit moduleExit(void){
    unregister_chrdev(MYMAJOR,"first_device_no");
    printk("Exiting module\n");

    // cdev_del(&first_device);
    // device_destroy(my_class,first_device_no);
    // class_destroy(my_class);
    // unregister_chrdev(first_device_no,DRIVER_NAME);
    // printk("Exiting module");
}

module_init(moduleInit);
module_exit(moduleExit);

