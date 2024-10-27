#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(){
    int fd= open("/dev/deviceB220160CS",O_RDWR);
    printf("opening was successfull\n");
    char write_buf[]="ANAGH_B220160CS\0";
    char read_buff[100];

    int bytes_written=write(fd,write_buf,strlen(write_buf));
    if(bytes_written) printf("Write successful\n");
    else printf("Write failed\n");
    int bytes_read= read(fd,read_buff,sizeof(read_buff)-1);
    if(bytes_read) printf("Read was successful\n");
    else printf("Read fail\n");
    read_buff[sizeof(read_buff)-1]='\0';
    printf("%s\n",read_buff);
    close(fd);
    printf("Closing device file\n");
    return 0;
}