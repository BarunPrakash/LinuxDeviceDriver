 #include "header.h"
 #include "declaration.h"
 ssize_t write_dev(struct file *filp,const char __user *buff,size_t size,loff_t *f_pos) 
 {
	int noctw,nocsw,ret;
        int i=0;
        int lv;
        int item=0;
	int res;
 	struct dev *ldev;
 	struct Qset *fqset;
        noctw=size;
        ret=0;
        nocsw=0;
 	ldev=filp->private_data;
 	if(!ldev)
 	{
		printk(KERN_ERR "memory is not created\n");
		goto OUT;
 	}
	fqset=NULL;
	if(down_interruptible(&ldev->sem))
	return -ERESTARTSYS;
	printk(KERN_INFO "begining of function:::::::%s",__func__);
	fqset=create_qsets(size); //Qset create...step-1
	if(!fqset)
	{
		printk(KERN_INFO "qset structure is not created\n");
		goto OUT;
	}
	res=create_qset_array(size,fqset); //step-2
	if(res==0)
	{
		printk(KERN_INFO "Qset is not creaded\n");
		goto OUT;
	}
	res=create_quantum(size,fqset);
	if(res==0)
	{
		printk(KERN_INFO "Quantums is not creaded\n");
		goto OUT;
	}
	ldev->qset=fqset;
	if(size>DEVICE_SIZE)
	{
		printk(KERN_INFO "device size is small\n");
		goto OUT;
	}
		item=size/quantum_size;
		if(size%quantum_size)
		item++;
		for(lv=0;lv<item;lv++)
		{
			if(noctw>quantum_size)
			noctw=quantum_size;
			ret=copy_from_user(fqset->data[i],buff+nocsw,noctw);
			if(ret)
			{
				printk(KERN_INFO "partialy return =%d\n",ret);
			
			}
			printk(KERN_INFO " %dquantum write=%s\n",i,(char*)fqset->data[i]);			
			nocsw=nocsw+noctw-ret;
			noctw=size-nocsw;			
			if(i==(qset_size-1))
        		{ 
                		if(fqset->next)
                		fqset=fqset->next;
                		i=0;
        		}
			else
        		i++;
        	}
			printk(KERN_INFO "total bytes written in this scull= %d\n",nocsw);			
		        printk(KERN_INFO "end>>>>>>>>>>>>write>>>\n");
			ldev->data_size=nocsw;
			up(&ldev->sem);
	return nocsw;
OUT:
	return -1;
 }
struct Qset *create_qsets(int size)
{
	struct Qset *first,*last;
	int noi,items,lv;
	items=qset_size*quantum_size;
	noi=size/items;
	if(size%items)
	noi++;
	printk(KERN_INFO "total number of bytes to be write in scull=%d",size);
	//printk(KERN_INFO "number of qset nodes formed is %d",noi);
	first=last=NULL;
	for(lv=0;lv<noi;lv++)
	{
		if(lv==0)
		{
			first=last=kmalloc(sizeof(struct Qset),GFP_KERNEL);
			if(!first)
			{
				printk(KERN_ERR "kmalloc failure");
				goto OUT;
			}
			first->next=NULL;
		}
		else
		{
			last->next=kmalloc(sizeof(struct Qset),GFP_KERNEL);
			if(!last->next)
			{
				printk(KERN_ERR "kmalloc failure in scull");
				goto OUT;
			}
			last=last->next;
			last->next=NULL;
		}
	}
	return first;
OUT:
	return 0;	 
  }
int create_qset_array(int size,struct Qset *lqset)
{
	int noqs,items,lv;
	items=qset_size*quantum_size;
	noqs=size/items;
	if(size%items)
	noqs++;
//	printk(KERN_INFO "number of qsets to be make =%d",noqs);
	for(lv=0;lv<noqs;lv++)
	{
		lqset->data=kmalloc((sizeof(void *)*qset_size),GFP_KERNEL);
		if(!lqset->data)
		{
			printk(KERN_ERR "lqset->data error");
			goto OUT;
		}
		if(lqset->next)
			lqset=lqset->next;
		//memset(lqset,'\0',sizeof(struct Qset));
	}
	return lv;
OUT:
	return 0;
 }
int create_quantum(int size, struct Qset *lqset)
{
	int noq,lv,i=0;
	noq=size/quantum_size;
	if(size%quantum_size)
	noq++;
	for(lv=0;lv<noq;lv++)
	{
		lqset->data[i]=kmalloc(quantum_size,GFP_KERNEL);
		if(!lqset->data[i])
		{
			printk(KERN_ERR "lscullqset->data[%d] allocation error",i);
			goto OUT;
		}	
		printk(KERN_INFO "quantum....=%d\n",i);
		if(i==(qset_size-1))
		{
			if(lqset->next)
			{
				
			//	printk(KERN_INFO "number of quantum made =%d\n",lv);
				lqset=lqset->next;
				i=0;
			}
		}
		else
		i++;
	}
	return lv;
OUT:
	return 0;
}


