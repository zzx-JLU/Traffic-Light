#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

int main() {
    int fd = open("/dev/s3c2440_led0",O_RDWR);
    if (fd < 0) {
        printf("####Led device open fail####\n");
        return (-1);
    }
	
    __asm__(
		"MOV R5,#0X011;"
		"STRB R5,[R4];"
    );

	// �ϱ���ƣ�0xffbe    10111110
	// ������ƣ�0xffdd    11011101
	unsigned int STATE[2] = {0xffbe, 0xffdd};

	// Ϊ����ʾ���㣬���Զ��л�ʱ������Ϊ3��
	while(1){
		for (int i = 0; i < 2; i++)
		{
			ioctl(fd, 0x12, STATE[i]);
			sleep(3);
		}
	}

    close(fd);
    return 0;
}