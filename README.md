# Linux-Character-Device-Driver
This repository contains a simple character device driver kernel module, that can be loaded dynamically to kernel

Firstmodule contains a character device driver that dynamically registers and obtains major and minor number. It automatically creates device file(first_device) in /dev

moduleB220160CS contains a character device driver that registers a character device module with major number 160 and obtains minor number(0 if major number not taken). we can load device file in /dev using 'sudo mknod /dev/deviceB220160CS c 160 {minor_number}' 

