// #include"declaration.h"
 #include"header.h"
 loff_t llseek_dev(struct file* filep, loff_t offset, int origin)
 {
	printk(KERN_INFO "filep->f_pos:::::::%d",(int)offset);
	switch(origin)
	{
		case SEEK_SET:
			filep->f_pos=offset;
			break;
		case SEEK_CUR:
			break;
		case SEEK_END:
			break;
		default:
			printk(KERN_INFO "in default\n");
	}
	printk(KERN_INFO "begin function.........%s",__func__);
	printk(KERN_INFO "filep->fpos::::::::::::%d",(int)filep->f_pos);
	printk(KERN_INFO "end function...........%s",__func__);
	return (int)filep->f_pos;
}
