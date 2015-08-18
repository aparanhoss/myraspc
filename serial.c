#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>
#define	LED	24
int main ()
{
	wiringPiSetupGpio();
	pinMode(LED, OUTPUT);
	
	
  int fd ;
	fd = serialOpen ("/dev/ttyAMA0", 9600);
	
	char c[]={0x28,0x00,0x07,0x02,0x30,0x01,0x62};
	int i=0;
	digitalWrite(LED, HIGH);	
	delay(10);
	for(i=0;i<7;i++){
		serialPutchar(fd,c[i]);
	}
	delay(10);
	digitalWrite(LED, LOW);
	for (;;)
	{
		printf("%.2x ",serialGetchar (fd));
		//putchar (serialGetchar (fd)) ;
		fflush (stdout) ;
	}
  return 0;

}
