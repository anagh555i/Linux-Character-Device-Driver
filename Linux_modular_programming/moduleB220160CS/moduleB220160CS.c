#include<linux/module.h>
#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h> 
#include<linux/fs.h>
#include<linux/utsname.h> // for getting current kernel version

#define MYMAJOR 160 // major number is 160

static int driver_open(struct inode *device_file,struct file *instance){
    printk("Open was called\n");
    return 0;
}

static int driver_close(struct inode *device_file,struct file *instance){
    printk("Close was called\n");
    return 0;
}

static char buffer[256];
static int buffer_pointer=0;

static ssize_t driver_read(struct file *File, char *user_buffer,size_t count,loff_t *offs){
    int to_copy, not_copied, delta;

    if(*offs >= buffer_pointer) return 0;

    to_copy= count<buffer_pointer?count:buffer_pointer;

    not_copied= copy_to_user(user_buffer,buffer,to_copy);

    delta= to_copy - not_copied;

    *offs += delta;

    printk("Read was called\n");
    return delta;
}

static ssize_t driver_write(struct file *File,const char *user_buffer,size_t count,loff_t *offs){
    int to_copy, not_copied, delta;

    to_copy= count<sizeof(buffer)?count:sizeof(buffer);

    not_copied= copy_from_user(buffer,user_buffer,to_copy);

    buffer_pointer= to_copy;

    delta= to_copy - not_copied;

    printk("Write was called\n");
    return delta;
}

static struct file_operations fops={
    .owner= THIS_MODULE,
    .open= driver_open,
    .release = driver_close,
    .read= driver_read,
    .write= driver_write
};


static int args[3]; // variable for taking kernel version;
module_param_array(args,int,NULL,S_IRUSR | S_IWUSR);

static int curr_version[3];

static void getCurrVersion(void){
    struct new_utsname *uts;
    uts=utsname();
    char *s;
    s=uts->release;
    printk("%s\n",s);
    int i=0,temp=0,j=0;
    while(i<3){
        temp=0;
        for(;s[j]>='0' && s[j]<='9';j++){
            temp*=10;
            temp+=(s[j]-'0');
        }
        j++;
        curr_version[i++]=temp;
    }
}

static int __init moduleInit(void){
    printk("Hello from kernel\n");
    getCurrVersion();
    for(int i=0;i<3;i++){
        printk("%d %d\n",args[i],curr_version[i]);
        if(args[i]<curr_version[i]){
            printk("Cannot load module to currVersion of Kernel");
            return -1;
        }
    }
    int retval= register_chrdev(MYMAJOR, "DeviceB220160CS",&fops); // here we have to manually allocate major number and later define device file manually
    printk("major number:%d minor_numer:%d\n",MYMAJOR,retval);
    return 0;
}

static void __exit moduleExit(void){
    unregister_chrdev(MYMAJOR,"DeviceB220160CS");
    printk("Exiting module\n");
}

module_init(moduleInit);
module_exit(moduleExit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Anagh");
MODULE_DESCRIPTION("Character Device driver module");