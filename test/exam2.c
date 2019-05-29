
#include<stdio.h>
#include<string.h>
//to insert padding between function, compile this file with -falign-functions=60 option.
//e.g., gcc -g -fno-pie -fno-pic -fno-pie -m32 -o test/exam2 test/exam2.c -static -falign-functions=60
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
int fun1(int a,int b){
	return a+b;
}
int fun2(int a,int b){
	return a*b;
}
int fun3(int a,int b){
	return a-b;
}
int fun4(int a,int b){
	return a/b;
}
int fun5(int a,int b){
	return a%b;
}
int fun6(int a,int b){
	return a&b;
}
int fun7(int a,int b){
	return a >> 2 + b <<2;
}
int fun8(int a,int b){
	return a|b;
}
int fun9(int a,int b){
	return (a+b)*233333 -0x2334;
}
int fun10(int a,int b){
	if(a>0){
		return a;
	}else{
		return -a;
	}
}
int fun11(int a,int b){
	
	return (a >> ((b <<2)%20));
}
int arry[100];
int data1,data2,data3;
int (*func[])(int, int) = { 
		fun1,fun2,fun3,fun4,fun5,fun6,fun7,fun8,fun9,fun10,fun11   
	}; 
int main(){
	int i,j,k;
	char input[20];
	readstr(input);
	data1=myatoi(input); //[0-2]
	readstr(input);
	data2=myatoi(input); //[0-20]
	readstr(input);
	data3=myatoi(input); //[0-10]
	for(i=0; i<(data2%11); i=i+data3){
		func[i] = func[i] - data1 ;
	}
	
	data3=func[0](data1,data2);

	data3=func[1](data1,data2);

	data3=func[2](data1,data2);

	data3=func[3](data1,data2);

	data3=func[4](data1,data2);

	data3=func[5](data1,data2);

	data3=func[6](data1,data2);

	data3=func[7](data1,data2);

	data3=func[8](data1,data2);

	data3=func[9](data1,data2);

	data3=func[10](data1,data2);
	return 0;
}

