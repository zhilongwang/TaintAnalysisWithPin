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
	uint i,j,cont=30;
	int p[4];
	char input[20];
	readstr(input);
	p[0]=myatoi(input);
	readstr(input);
	p[1]=myatoi(input);
	readstr(input);
	p[2]=myatoi(input);
	if(p[0]%2==0){
		cont++;
	}
	if(p[1]%3==0){
		cont++;
	}
	if(p[2]%3==0){
		cont++;
	}
	if((p[0]<p[1]) || (p[0]-p[1]) < 5 ){
		p[0]++;
	}else{
		p[1]++;
		cont++;
	}
	if((p[0]+p[1])<10 && (p[1]-p[0]) > 5 ){
		p[2]=(p[0]+p[1])/2;
		cont++;
	}
	else{
		p[2]=(p[0]+p[1]+p[2])/2;
	}
	for(i=0;i<2;i++){	
		if(cont>p[1]){
			if(j>i){
				if((cont<i)||cont>j){
					cont++;
				}
			}
		}
	}
	return 0;
}
