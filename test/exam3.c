#include<stdio.h>
#include<string.h>
#include<stdlib.h>
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
int main(){
	uint i,j=30;
	int p[4];
	char input[20];
	readstr(input);
	p[0]=myatoi(input);
	readstr(input);
	p[1]=myatoi(input);
	readstr(input);
	int len=strlen(input);
	p[2]=myatoi(input);
	p[2]=32;
	p[3]=p[2];
	for(i=3;i<p[3];i++){
		if(p[2]%i==0){
			p[i]=p[0];
		}
	}
	for(i=0;i<p[0];i++){	
		if(j>p[1]){
			if(j>i){
				if((len<i)||len>j){
					len++;
				}
			}
		}
	}
	return 0;
}
