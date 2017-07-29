 #include <stdio.h>
 #include <stdlib.h>
 #include <fcntl.h>
 #include <string.h>
 int main(int argc, char *argv[])
 {
        int fd;
        int len;
	int ret;
        char buffii[1024];
        fd=open(argv[1],O_RDONLY);
        if(fd<0)
        {
                printf("error open file and fd is not created\n");
                goto OUT;
        }
        printf("reader_application fd=%d\n",fd);
	lseek(fd,20,SEEK_SET);
        ret=read(fd,buffii,1024);
        printf("total data size read by reader=%d\n",ret);
        printf("total data to be read by reader=%s\n",buffii);
        return 0;
 OUT:
        return -1;
 }

