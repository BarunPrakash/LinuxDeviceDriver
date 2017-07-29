 int close_dev(struct inode *inodep, struct file *filep);
 int open_dev(struct inode *inodep, struct file *filep);
 ssize_t read_dev(struct file *filp,const char __user *buff,size_t size,loff_t *f_pos);
 ssize_t write_dev(struct file *filp,const char __user *buff,size_t size,loff_t *f_pos);
 loff_t llseek_dev(struct file* filep, loff_t offset, int origin);
 struct file_operations fops={
                                open:open_dev,
                                release:close_dev,
				write:write_dev,
				read:read_dev,
				llseek:llseek_dev
                             };
