#include<stdio.h>
#include<string.h>
char * readstr(char *str){
	char c;
	int i=0;
	while((c=getchar()) !='\n'){
		str[i]=c;
		i++;
	}
	str[i]='\0';
	return str;
}
int myatoi(char *str)  
{  
        if(!str)  
                return -1;  
        int bMinus=0;  
        int result=0;  
  
        if(('0'>*str || *str>'9')&&(*str=='+'||*str=='-'))  
        {  
               if(*str=='-')  
               bMinus=1;  
               *str++;  
        }  
        while( *str != '\0')  
        {  
                if('0'> *str || *str>'9')  
                        break;  
                else  
                        result = result*10+(*str++ - '0');  
        }  
  
        if (*str != '\0')//no-normal end  
                return -2;  
  
        return bMinus?-result:result;  
}
int padding[16];
int arry[100];
int data1,data2;
int main(){
	printf("arry address:%x\n",&arry[0]);
	int i,j,k;
	char input[20];
	readstr(input);
	data1=myatoi(input);
	readstr(input);
	data2=myatoi(input);
	for(i=4;i<8;i++){
		arry[i]=data1;
	}
	for(i=4;i<8;i=i+2){
		arry[i]=10;
	}
	for(i=8;i<12;i=i+2){
		arry[i]=data1*10;
	}
	for(i=12;i<16;i=i+3){
		arry[i]=data1%10;
	}
	for(i=16;i<20;i++){
		arry[i]=4+data2;
	}
	data1=12;
	data2=11;
	for(i=20;i<24;i++){
		arry[i]=data1+data2;
	}
	char *r=&data1;
	char *q=r;
	char *p=q;
	*(p+3)=(char)arry[5];
	*(p+1)=(char)arry[5];
	return 0;
}
