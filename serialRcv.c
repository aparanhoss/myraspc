#include <wiringSerial.h>
#include <wiringPi.h>
#include <stdio.h>
#define	LED	24
int count=0;
int main (int argc,char *argv[])
{
        char syscmd[100];
        
	int fd ;
	wiringPiSetupGpio();
	pinMode(LED, OUTPUT);
        
        int completa=0;
        
	char c[3+argc];
        char rcv[50];
        rcv[2]=50;
        
	c[0]=0x28;
	c[1]=0x00;
	c[2]=sizeof(c);
	int i=0;
	char tmp=0;
        
	for(i=1;i<argc;i++){
		sscanf(argv[i],"%x",&tmp);
		c[2+i]=tmp;
	}
  
	fd = serialOpen ("/dev/ttyAMA0", 9600);
	
	//char c[]={0x28,0x00,0x07,0x04,0x30,0x00,0x63};
	
	//int i=0;
	//c[2]=sizeof(c);
	printf("%d\n",sizeof(c));
        /*
	digitalWrite(LED, HIGH);	
        c[(c[2]-1)]=calcCheckSum(c);
	delay(10);
        sendMsg(fd,c);
        /*
//	int sum=0;
	for(i=0;i<(c[2]-1);i++){
		serialPutchar(fd,c[i]);
		//sum+=c[i];
		//c[(c[2]-1)]+=c[i];
		
	}
		///c[(c[2]-1)]=sum&0xff;
    
	serialPutchar(fd,c[(c[2]-1)]);
        */
        /*
	delay(10);
	digitalWrite(LED, LOW);
        */
        sendProtocol(fd,c);
        
	for(i=0;i<c[2];i++){
		printf("%.2x ",c[i]);
	}
	printf("\n");
        char ch;
	while(1)
	{
                //rcvDado(fd,c);
                ch=serialGetchar(fd);
                if(ch>=0 && ch<=0xff){
                    rcv[count]=ch;
                    if(rcv[0]==0x28){
                        //printf("%.2x ",c[count]);
                        //printf("%d ",count);
                        count++;
                        if(count==rcv[2]){
                            completa=1;
                            count=0;
                        }
                        
                    }
                    else{
                        count=0;
                    }
                    
                }
		
                if(count>49){
                    count=0;
                }
		//putchar (serialGetchar (fd)) ;
		fflush (stdout) ;
                if(completa)
                {
                    completa=0;
                    printf("Rcv: ");
                    printMsg(rcv);
                    printf("CMD %.2x\n",rcv[4]);
                    printf("FROM %.2x\n",rcv[5]);
                    if(rcv[4]==0x42){
                        if(rcv[5]>=0x01 && rcv[5]<=0x0f){
                            //printf("From LFI %d\n",rcv[5]%0x0f);                            
                            //printf("Saida 00 %.2x%.2x",rcv[6],rcv[7]);
                            printf("Saida 00 %d\n",(rcv[6]<<8)+rcv[7]);
                            sprintf(syscmd,"sed -i \"3s/.*/value=%d><br>/\" /var/www/index.html",(rcv[6]<<8)+rcv[7]);
                            system(syscmd);
                            printf("Saida 01 %d\n",(rcv[8]<<8)+rcv[9]);
                            sprintf(syscmd,"sed -i \"5s/.*/value=%d><br>/\" /var/www/index.html",(rcv[8]<<8)+rcv[9]);
                            system(syscmd);
                            printf("Saida 02 %d\n",(rcv[10]<<8)+rcv[11]);
                            sprintf(syscmd,"sed -i \"7s/.*/value=%d><br>/\" /var/www/index.html",(rcv[10]<<8)+rcv[11]);
                            system(syscmd);
                            fflush(stdout);
                        }
                    }
                    
                }
	}
  return 0;

}

int calcCheckSum(char in[]){
        int i=0;
        int len=in[2]-1;
        int sum=0;
        for(i=0;i<len;i++){
            sum+=in[i];
            
        }		
        return sum&0xff;
}

int testCheckSum(char in[]){
	return (in[(in[2]-1)==calcCheckSum(in)]);
}

int sendMsg(int fd,char in[]){
    int i=0;
    for(i=0;i<in[2];i++){
        serialPutchar(fd,in[i]);
    }
    return 0;
}
int sendProtocol(int fd,char in[]){
    in[(in[2]-1)]=calcCheckSum(in);
    digitalWrite(LED, HIGH);	    
    delay(10);
    sendMsg(fd,in);
    delay(8);
    digitalWrite(LED, LOW);
}

int rcvDado(int fd,char in[]){
    char c=serialGetchar(fd);
    if(c>=0 && c<=0xff){
        in[count]=c;
        count++;
    }
}


int printMsg(char msg[]){
    int i=0;
    for(i=0;i<msg[2];i++){
        printf("%.2x ",msg[i]);
    }
    printf("\n");
    return 0;
}
int map(int x,int x1,int y1,int x2,int y2){
    
    if(x<x1){return x2;}
    else if(x>y1){return y2;}
    else{
      int y;
      return y;
    }
}