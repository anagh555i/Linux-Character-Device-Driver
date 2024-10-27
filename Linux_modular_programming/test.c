#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(){
    int fd= open("/dev/firstdevice",O_RDWR);
    printf("opening was successfull");
    char write_buf[]="Text to write\0";
    char read_buff[100];

    int bytes_not_written=write(fd,write_buf,strlen(write_buf));
    int bytes_not_read= read(fd,read_buff,sizeof(read_buff)-1);
    read_buff[sizeof(read_buff)-1]='\0';
    printf("%s\n",read_buff);
    close(fd);
}