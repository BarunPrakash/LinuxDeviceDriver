#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>

int main()
{
	unsigned char byte;
	int fd,count,last=1;
	fd = open("node",O_WRONLY);
	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	byte=1;
	while(1)
	{
		count = write(fd,&byte,1);
		printf("Main : Count = %d\n",count);
		printf("data write = %d",byte);
		sleep(1);
		byte<<=1;

		if(byte == 0)
		byte = 1;		
//		last++;
//		if(last==10)
//		break;
	}

	return 0;
}
