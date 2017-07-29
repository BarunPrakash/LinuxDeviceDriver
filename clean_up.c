 #include "header.h"
 #include "declaration.h"
 static void clean_function(void)
 {
	int dev_no;
	for(dev_no=0;dev_no<nod;dev_no++)
	{
		cdev_del(&sdev[dev_no].c_dev);
	}
        unregister_chrdev_region(dev,nod);
        printk(KERN_ALERT"bye device driver world\n");
 }
 module_exit(clean_function);
