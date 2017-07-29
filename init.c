 #include "header.h"
 #include "declaration.h"
 #include "file_opr.h"
 //int function();
 int major_no;
 int minor_no;
 int nod;
 int qset_size;//=QSET_SIZE;
 int quantum_size;//=QUANTUM_SIZE;
 int data_size=DATA_SIZE;
 int device_size=DEVICE_SIZE;
 dev_t dev;
 dev_t first;
 module_param(nod,int,S_IRUGO);
 module_param(qset_size,int,S_IRUGO);
 module_param(quantum_size,int,S_IRUGO);
 struct dev *sdev;
 static int init_function(void)
 {
        int res,i;
        minor_no=0;
        major_no=0;
	function();
	if (major_no)
	{
                first = MKDEV(dev,1);
                res = register_chrdev_region(first,nod, DVNAME);
	}
        else
	{
                res = alloc_chrdev_region(&dev,0,nod,DVNAME);
	}
        if(res!=0)
        {
                printk(KERN_ERR "error in registering driver\n");
                goto OUT;
        }
        sdev = (struct dev *)kmalloc(sizeof(struct dev)*nod,GFP_KERNEL);
        if(sdev == NULL)
        {
                printk(KERN_ERR "memory not allocated\n");
            	goto OUT;
        }
        memset(sdev,'\0',sizeof(struct dev)*nod);
        major_no = MAJOR(dev);
        for(i=0;i<nod;i++)
        {
                first=MKDEV(major_no,i);
                cdev_init(&sdev[i].c_dev,&fops);//init
                sdev[i].c_dev.owner = THIS_MODULE;
                sdev[i].c_dev.ops = &fops;
                sdev[i].qset_size = qset_size;
                sdev[i].quantum_size = quantum_size;
                sdev[i].data_size = data_size;
                sdev[i].device_size = device_size;
		sema_init(&sdev[i].sem,1);				//semaphore
                cdev_add(&sdev[i].c_dev,first,1);//adding
                minor_no = MINOR(first);
                printk(KERN_INFO "minor number=%d \n",minor_no);
                printk(KERN_INFO "major number=%d \n",major_no);
	}       
        //printk(KERN_INFO "major number=%d \n",major_no);
        printk(KERN_INFO"total no. of devices= %d\n",nod);     
        return 0;
OUT:
	return -1;
}
module_init(init_function);

