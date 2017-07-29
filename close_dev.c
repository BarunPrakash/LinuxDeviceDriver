 #include "header.h"
 #include "declaration.h"
 int close_dev(struct inode *inodep, struct file *filep)
 {
        printk(KERN_INFO "close file successfully>>>>>>%s",__func__);
        return 0;
 }     
