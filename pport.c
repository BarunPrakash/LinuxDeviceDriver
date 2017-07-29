#include"header.h"
static struct class *dev_class;

ssize_t pport_write(struct file *filep,const char __user *ubuff,size_t count,loff_t *fops)
{
	char pport_buffer;
	int retval;

	#ifdef DEBUG
	printk(KERN_INFO "Begin : %s\n",__func__);
	#endif

	retval = copy_from_user(&pport_buffer,ubuff,1);
	printk(KERN_INFO "CHAR : %c\n",pport_buffer);
	printk(KERN_INFO "retval : %d\n",retval);


	outb(pport_buffer,DATA);	

	#ifdef DEBUG
	printk(KERN_INFO "Begin : %s\n",__func__);
	#endif
	return retval;
}

int pport_open(struct inode *inodep, struct file *filep)
{
	struct pport_dev *lpportdev;
	#ifdef DEBUG
	printk(KERN_INFO "Begin : %s\n",__func__);
	#endif

	lpportdev = container_of(inodep->i_cdev,struct pport_dev,pport_cdev);
	if(!lpportdev)
	{
		#ifdef DEBUG
		printk(KERN_INFO "Container_of failed.\n");
		#endif
		goto OUT;
	}
	filep->private_data = lpportdev;
	
	if((filep->f_flags & O_ACCMODE) == O_WRONLY)
	{
		#ifdef DEBUG
		printk(KERN_INFO "File open in write mode");
		#endif
	}

	if((filep->f_flags & O_ACCMODE) == O_RDONLY)
	{
		#ifdef DEBUG
		printk(KERN_INFO "File open in read mode");
		#endif
	}

	#ifdef DEBUG
	printk(KERN_INFO "END : %s\n",__func__);
	#endif
	return 0;

OUT:
	#ifdef DEBUG
	printk(KERN_INFO "%s returns -1\n",__func__);
	#endif
	return -1;
}


int pport_release(struct inode *inodep, struct file *filep)
{
	#ifdef DEBUG
	printk(KERN_INFO "Begin : %s\n",__func__);
	#endif
	

	#ifdef DEBUG
	printk(KERN_INFO "END : %s\n",__func__);
	#endif
	return 0;
}

int pport_setup(struct pport_dev *fpportdev,int flv)
{
	int retval;
	dev_t devno;

	fpportdev[flv].size_qset = pport_qset;
	fpportdev[flv].size_quantum = pport_quantum;
	fpportdev[flv].pport_cdev.owner = THIS_MODULE;
	fpportdev[flv].pport_cdev.ops = &fops;

	cdev_init(&pportdev->pport_cdev,&fops);

	devno = MKDEV(major,flv+minor);
	retval = cdev_add(&pportdev->pport_cdev,devno,1);
	if(retval < 0)
	{
		#ifdef DEBUG
		printk(KERN_INFO "ERROR in cdev_add");
		#endif
	}
	#ifdef DEBUG
	printk(KERN_INFO "Device %d added successfully.(Major No. %d, Minor No. %d)\n",flv,MAJOR(devno),MINOR(devno));
	#endif

	
	return 0;
}

static int __init pport_initialise(void)
{
	int ret,lv;
	void *retval;
//	static struct class *dev_class;

	#ifdef DEBUG
	printk(KERN_INFO "Begin : %s\n",__func__);
	#endif

	if(major)
	{
		dev_new = MKDEV(major,minor + 1);
		ret = register_chrdev_region(dev_new,1,DEV_NAME);
		if(ret)
		{
			#ifdef DEBUG
			printk(KERN_INFO "register_chrdev_region unsuccessful.\n ");
			#endif
		}
		#ifdef DEBUG
		printk(KERN_INFO "Allocated Major No = %d\n",MAJOR(dev));
		#endif
		#ifdef DEBUG
		printk(KERN_INFO "Allocated Minor No = %d\n",MINOR(dev));
		#endif
	}
	ret = alloc_chrdev_region(&dev, minor, 1, DEV_NAME);
	if(ret != 0)
	{
		#ifdef DEBUG
		printk(KERN_INFO "alloc_chrdev_region unsuccessful.\n ");
		#endif
	}
	#ifdef DEBUG
	printk(KERN_INFO "alloc_chrdev_region successful.");
	#endif

	major=MAJOR(dev);
	minor=MINOR(dev);
	#ifdef DEBUG
	printk(KERN_INFO "Allocated Major No = %d\n",major);
	#endif
	#ifdef DEBUG
	printk(KERN_INFO "Allocated Minor No = %d\n",minor);
	#endif

	pportdev = (struct pport_dev *)kmalloc(sizeof(struct pport_dev),GFP_KERNEL);
	if(!pportdev)
	{
		#ifdef DEBUG
		printk(KERN_INFO "kmalloc failed to allocate memory.\n");
		#endif
	}
	#ifdef DEBUG
	printk(KERN_INFO "kmalloc allocates memory successfully.\n");
	#endif
	memset(pportdev,'\0',sizeof(struct pport_dev));

	for(lv=0; lv < no_of_dev; lv++)
	{
		pport_setup(pportdev,lv);
	}



	ret = check_region(DATA,3);
	if(ret<=0)
	{
		#ifdef DEBUG
		printk(KERN_INFO "Port is not available\n");
		#endif

		release_region(DATA,3);

		#ifdef DEBUG
		printk(KERN_INFO "I/O Port returned to system\n");
		#endif
	}
	retval = (void *)request_region(DATA,3,DEV_NAME);
	if(retval==NULL)
	{
		#ifdef DEBUG
		printk(KERN_INFO "Port Allocation Failed\n");
		#endif
	}
	#ifdef DEBUG
	printk(KERN_INFO "Port Allocation Success.\n");
	#endif


	dev_class = class_create(THIS_MODULE, "VIRTUAL");
	device_create(dev_class,NULL,dev,NULL,"%s","a_node");
	#ifdef DEBUG
	printk(KERN_INFO "End : %s\n",__func__);
	#endif
	return 0;
}

static void pport_cleanup(void)
{
	#ifdef DEBUG
	printk(KERN_INFO "Begin : %s\n",__func__);
	#endif

	cdev_del(&pportdev->pport_cdev);
	#ifdef DEBUG
	printk(KERN_INFO "cdev_del successful.");
	#endif
	
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	
	release_region(DATA,3);
	#ifdef DEBUG
	printk(KERN_INFO "I/O Port returned to system\n");
	#endif

	unregister_chrdev_region(dev,1);
	#ifdef DEBUG
	printk(KERN_INFO "unregister_chrdev_region successful.");
	#endif

	kfree(pportdev);
	#ifdef DEBUG
	printk(KERN_INFO "Memory is freed from pport_dev\n");
	#endif

	#ifdef DEBUG
	printk(KERN_INFO "End : %s\n",__func__);
	#endif

}

module_init(pport_initialise);
module_exit(pport_cleanup);
