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
 int trim_dev(struct dev *scull_dev )
 {
	int i=0;
        printk(KERN_INFO "trim...........bigining.........\n");
	struct Qset *scullqset,*sec_last;
	sec_last=scullqset=scull_dev->qset;
	if(!scullqset)
	{
        	printk(KERN_ERR "scull not found..............................\n");
		goto OUT;
	}
	if(down_interruptible(&scull_dev->sem))
        return -ERESTARTSYS;
/*	if(scullqset)
	{
		if(scullqset->data)
		{
		for(i=0;i<qset_size;i++)
                {
                        kfree(scullqset->data[i]);
                        scullqset->data[i]=NULL;
                }
	 		kfree(scullqset->data);
                        scullqset->data=NULL;
		}
		else
        	printk(KERN_ERR "data not found in scull\n");
			
	}*/
	//scullqset=scullqset->next;
 	while(scullqset->next)
	{
		if(scullqset->data)
		{
			for(i=0;i<qset_size;i++)
			{
			kfree(scullqset->data[i]);
			scullqset->data[i]=NULL;
			}
			kfree(scullqset->data);
			scullqset->data=NULL;
		}
	sec_last=scullqset->next;
        kfree(scullqset);
        scullqset=NULL;
        scullqset=sec_last;
	}
	kfree(scullqset);
	scullqset=NULL;
	up(&scull_dev->sem);
        printk(KERN_INFO "trim...........Ending....................\n");
        return 0;
OUT:
	return -1;

 }

