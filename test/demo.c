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
	signed int x, x2;
	scanf("%x", &x);
	unsigned int y2, y;
	y = (unsigned int)x;
	y = y2 * y;
	x = x2 * x;
       return 0;

}
int padding[16];
int arry[100];
int data1,data2;
int main(){
	int i,j,k;
	char input[30];
	printf("1");
	readstr(input);
	printf("2");
	data1=myatoi(input);
	printf("3");
	readstr(input);
	printf("4");
	data2=myatoi(input);
	for(i=2;i<(data1%8);i++){
		arry[i]=data1;
		printf("1");
	}
	for(i=2;i<(data1%8);i=i+2){
		arry[i]=10;
		printf("2");
	}
	for(i=8;i<8+(data2%8);i=i+2){
		arry[i]=data1*10;
		printf("3");
	}
	for(i=16;i<24;i=i+3){
		arry[i]=data1%10;
		printf("4");
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
