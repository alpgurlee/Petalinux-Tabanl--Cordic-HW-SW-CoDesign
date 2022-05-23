#define TERMINAL "/dev/ttyPS0"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/mman.h>
#include <math.h>
#include <time.h>

char karakter[255];
int x[255];
int y[255];
int i;
int j;

int main()
{
	unsigned int gpio_size = 0x8000;
	off_t gpio_pbase = 0x41200000;
	char *portname = TERMINAL;
	int fd;
	int wlen;
	long long *gpio64_vptr;
	int fd1;
	long long* bram64_vptr;

	
	char *xstr = "Sayilari girin \n";
	int xlen = strlen(xstr);
fd = open (portname, O_RDWR | O_NOCTTY| O_SYNC);
if (fd < 0) {
	printf("Error opening %s: %s\n", portname, strerror(errno));
	return 0;
}
	wlen = write(fd, xstr, xlen); 
	if (wlen != xlen){
		printf("Error from write: Xd, %d\n", wlen, errno);
	}

do{
	unsigned char buf[80];
	int rdlen;

	rdlen = read (fd, buf, sizeof(buf) -1); 
	if (rdlen > 0){
	int flag = 0;
    j = strlen(buf);
	buf[rdlen]=0;
	printf(" sayilar: %s \n", buf);
	
	
    for (i = 0; i < j; i++) 
    {
        if(buf[i]== '-'){ 
            flag = 1;
        } else{
            if(flag == 0){
                x[i] = buf[i] - '0'; 
            }
            else {
                y[i] = buf[i] - '0';
            }
        }
    }

    int ilkDeger = x[1] + x[0]*10; 
    int ikinciDeger = y[4] + y[3]*10;
    printf("Deger1: %d  Deger2:%d \n", ilkDeger, ikinciDeger);
	
	if ((fd1 = open("/dev/mem", O_RDWR | O_SYNC)) != -1) {

		gpio64_vptr = (long long *)mmap(NULL, gpio_size,PROT_READ|
		PROT_WRITE, MAP_SHARED, fd1, gpio_pbase); 
		*(gpio64_vptr)  = ilkDeger; 
		*(gpio64_vptr + 1) = ikinciDeger; 
	
		int sonuc = *(gpio64_vptr + 1); 
		printf("sonuc: %d \n:" , sonuc);
		
		
		char sonuc2[20];
		sprintf(sonuc2, "%d", sonuc);
		long long  Glen = strlen(sonuc2);
		write(fd, sonuc2, Glen); 
		close(fd1);
		}
	} else if (rdlen < 0) {
		printf("Error from read: %d: %s\n", rdlen, strerror (errno));
	}else {
		printf("Timeout from read\n");
	}

	}while (1);
}