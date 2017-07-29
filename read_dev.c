 #include "header.h"
 #include "declaration.h"
 ssize_t read_dev(struct file *filp,const char __user *buff,size_t size,loff_t *ff_pos) 
 {
 	struct dev *ldev;
 	struct Qset *fqset;
	int noctr,nocsr,ret;
        int i=0;
	int size_n=0;
        int lv;
	int move;
	//int quantum_move;
        int item=0;
        nocsr=0;
	//int wv=0;
        ret=0;
	*ff_pos=filp->f_pos;
 	ldev=filp->private_data;
 	if(!ldev)
 	{
		printk(KERN_ERR "memory is not created\n");
		goto OUT;
 	}
	fqset=NULL;
	printk(KERN_INFO "value of offset=%d\n",(int*)*ff_pos);
	printk(KERN_INFO "size of size =%d\n",(int*)size);
	printk(KERN_INFO "begining of reading function:::::::%s",__func__);
	if(down_interruptible(&ldev->sem))
        return -ERESTARTSYS;
	fqset=ldev->qset;
        noctr=ldev->data_size;
	size_n=noctr;
	move =*ff_pos/(qset_size*quantum_size);
	//quantum_move =*ff_pos%(qset_size*quantum_size);
	/*while(wv<move)
	{
		if(fqset->next)
		fqset=fqset->next;
		wv++;
	}*/
	//size_n =size_n-*ff_pos;
	printk(KERN_INFO "size of size_n=%d",size_n);
	if(size<size_n)
	{
		printk(KERN_INFO "device size is small\n");
		goto OUT;
	}
	item=size_n/quantum_size;
	if(size_n%quantum_size)
	item++;
	for(lv=0;lv<item;lv++)
	{
		if(noctr>quantum_size)
		noctr=quantum_size;
		//if(lv==0)
		//noctr=quantum_move;
		ret=copy_to_user(buff+nocsr,fqset->data[i],noctr);
		if(ret)
		{
			printk(KERN_INFO "partialy read =%d\n",ret);	
		}
		printk(KERN_INFO " %dquantum read=%s\n",i,(char*)fqset->data[i]);			
		nocsr=nocsr+noctr-ret;
		noctr=size_n-nocsr;
	//	printk(KERN_INFO " %d total nocsw=%dand noctw=%d\n",i,nocsr,noctr);			
		if(i==(qset_size-1))
        	{ 
                	if(fqset->next)
                	fqset=fqset->next;
                	i=0;
        	}
		else
        	i++;
        }
			printk(KERN_INFO "total bytes read by application= %d\n",nocsr);			
		        printk(KERN_INFO "end>>>>>>>>>>>>read>>>\n");
	up(&ldev->sem);
	return nocsr;
OUT:
	return -1;
 }
