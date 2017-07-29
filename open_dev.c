 #include "header.h"
 #include "declaration.h"
 int trim_dev(struct dev*);
 int open_dev(struct inode *inodep, struct file *filep)
 {
        struct dev *scull_dev;
        int ret;
        printk(KERN_INFO "begining function....%s",__func__);
        scull_dev = NULL;
        scull_dev=container_of(inodep->i_cdev,struct dev,c_dev);
        if(!scull_dev)
        {

                printk(KERN_ERR "scull_dev not found\n");
                goto OUT;
        }
        filep->private_data=scull_dev;
        if((filep->f_flags&O_ACCMODE)==O_WRONLY)
        {
                printk(KERN_INFO "device has been successfully opened in write mode\n");
                ret=trim_dev(scull_dev);
                if(ret<0)
                {
                        printk(KERN_ERR "trimming of device is not completed\n");
                        goto OUT;
                }
                printk(KERN_INFO "trimming of device done\n");
        }
        printk(KERN_INFO "scull_dev found successfully");
        printk(KERN_INFO "end function....%s",__func__);
        return 0;
OUT:
        return 0;
 }
 int trim_dev(struct dev *fsdev )
 {
	struct Qset *lqset,*slqset;
        int lv;
        if(down_interruptible(&fsdev->sem))
        return -ERESTARTSYS;
        #ifdef DEBUG
        printk(KERN_INFO "Begin:%s",__func__);
        #endif
        if(!fsdev)
        {
                #ifdef DEBUG
                printk(KERN_ERR "No device found\n");
                #endif
        }
        if(!fsdev->qset)
        {
                #ifdef DEBUG
                printk(KERN_ERR "Nothing to trim\n");
                #endif
        }
 	lqset=slqset=fsdev->qset;
        while(fsdev->qset)
        {
                lqset=slqset=fsdev->qset;
                while(lqset->next)
                {
                        slqset = lqset;
                        lqset=lqset->next;
                }
                if(lqset->data)
                {
                        for(lv=0;lv<qset_size;lv++)
                        {
                                kfree(lqset->data[lv]);
                                lqset->data[lv]=NULL;
                        }
                        kfree(lqset->data);
                        lqset->data=NULL;
                        kfree(lqset);
                        lqset=NULL;
                }
                if(fsdev->qset->next)
                {
         		slqset->next = NULL;
                }
                else
                {
                        slqset=NULL;
                   
                        fsdev->qset=NULL;
                       
                }
        }
        up(&fsdev->sem);
	return 0;
}

