 #include <stdio.h>
 #include <stdlib.h>
 #include <fcntl.h>
 #include <string.h>
 int main(int argc, char *argv[])
 {
	int fd;
	int len;
	char buff[]="emblogic embedded system pvt.ltd noida up india if you want to add something you feelfree to add";
	fd=open(argv[1],O_WRONLY);
	len=strlen(buff);
	if(fd<0)
	{
		printf("error open file and fd is not created\n");
		goto OUT;
	}
	printf("writer_application_fd=%d\n",fd);
	printf("total data to be write in buffer=%d\n",len);
	printf("data write in buffer=%s\n",buff);
	write(fd,buff,len);
	return 0;
 OUT:
	return -1;
 }
