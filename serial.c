#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>
#define	LED	24
int main (int argc,char *argv[])
{
	
	wiringPiSetupGpio();
	pinMode(LED, OUTPUT);
	char c[3+argc];
	c[0]=0x28;
	c[1]=0x00;
	c[2]=sizeof(c);
	int i=0;
	char tmp=0;
	for(i=1;i<argc;i++){
		sscanf(argv[i],"%x",&tmp);
		c[2+i]=tmp;
	}
  int fd ;
	fd = serialOpen ("/dev/ttyAMA0", 9600);
	
	//char c[]={0x28,0x00,0x07,0x04,0x30,0x00,0x63};
	
	//int i=0;
	//c[2]=sizeof(c);
	printf("%d\n",sizeof(c));
	digitalWrite(LED, HIGH);	
	delay(10);
	for(i=0;i<(c[2]-1);i++){
		serialPutchar(fd,c[i]);
		c[(c[2]-1)]+=c[i];
	}
	serialPutchar(fd,c[(c[2]-1)]);
	delay(10);
	digitalWrite(LED, LOW);
	for(i=0;i<c[2];i++){
		printf("%.2x ",c[i]);
	}
	printf("\n");
	for (;;)
	{
		printf("%.2x ",serialGetchar (fd));
		//putchar (serialGetchar (fd)) ;
		fflush (stdout) ;
	}
  return 0;

}
