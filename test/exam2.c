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
int arry[100];
int data1,data2,data3;
int (*func[])(int, int) = { 
		fun1,fun2,fun3,fun4,fun5  
	}; 
int main(){
	printf("num1:%d\n",func[0]);
	printf("num2:%d\n",func[1]);
	printf("num3:%d\n",func[2]);
	printf("num4:%d\n",func[3]);
	printf("num5:%d\n",func[4]);
	int i,j,k;
	char input[20];
	readstr(input);
	data1=myatoi(input);
	readstr(input);
	data2=myatoi(input);
	if(data1==func[0]){
		func[3]=data1;
		data3=func[0](data1,data2);
		printf("%d\n",data3);
	}else if(data1==func[1]){
		func[1]=data1;
		data3=func[1](data1,data2);
		printf("%d\n",data3);
	}else if(data1==func[2]){
		func[4]=data1;
		data3=func[2](data1,data2);
		printf("%d\n",data3);
	}else if(data1==func[3]){
		func[2]=data1;
		data3=func[3](data1,data2);
		printf("%d\n",data3);
	}else if(data1==func[4]){
		func[0]=data1;
		data3=func[4](data1,data2);
		printf("%d\n",data3);
	}
	return 0;
}