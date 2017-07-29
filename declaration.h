extern int major_no;
extern int minor_no;
extern int nod;
extern dev_t dev;
#define DVNAME "ch1_driver"
#define MAJORNO 0
#define MINORNO 0
# ifndef DEBUG
# define DEBUG 0
# endif

# ifndef QSET_SIZE
# define QSET_SIZE 8
# endif

# ifndef QUANTUM_SIZE
# define QUANTUM_SIZE 3
# endif

# ifndef DATA_SIZE
# define DATA_SIZE 0
# endif

# ifndef DEVICE_SIZE
# define DEVICE_SIZE 1024
# endif
//int open_dev(struct inode *inodep, struct file *filep);
//int close_dev(struct inode *inodep, struct file *filep);
//size_t write_dev(struct file *filp,const char __user *buff,size_t size,loff_t *f_pos);
struct Qset
        {
                struct Qset *next;
                void **data;
        };
struct dev
{
        struct Qset *qset;
        struct cdev c_dev;
	int qset_size;
        int quantum_size;
        int data_size;
        int device_size;
	struct semaphore sem;

};
extern struct dev *sdev;
extern int quantum_size,qset_size;
extern int data_size,device_size;

struct Qset *create_qsets(int size);
int create_qset_array(int size,struct Qset *);
int create_quantum(int size,struct Qset *);

