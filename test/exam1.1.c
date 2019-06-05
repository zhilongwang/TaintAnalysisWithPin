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
	int i,j,k;
	char input[30];
	readstr(input);
	data1=myatoi(input);
	readstr(input);
	data2=myatoi(input);
	for(i=2;i<(data1%8);i++){
		arry[i]=data1;
	}
	for(i=2;i<(data1%8);i=i+2){
		arry[i]=10;
	}
	for(i=8;i<8+(data2%8);i=i+2){
		arry[i]=data1*10;
	}
	for(i=16;i<24;i=i+3){
		arry[i]=data1%10;
	}
	data1=12;
	data2=11;
	for(i=2;i<24;i=i+4){
		arry[i]=data1+data2;
	}
	char *r= (char *)(&data1);
	char *q=r;
	char *p=q;
	*(p+3)=(char)arry[5];
	*(p+1)=(char)arry[5];
	return 0;
}
