#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


int main()
{
        int fd;
        int num1,num2;
         printf("Enter two numbers to write into driver :");
        scanf("%d %d",&num1,&num2);
        int ubuff[2]={num1,num2};

        fd = open("/dev/write1_device", O_RDWR,0777);
        if(fd > 0) {
                printf("Device File Opened successfully in the path /dev/mydriver_device...\n");
        }
        else {
                printf("Cannot open device file...\n");
                return 0;
        }
        
        printf("Data Writing ...");
        write(fd,(char *)&ubuff, sizeof(ubuff));
        close(fd);
        return 0;
}