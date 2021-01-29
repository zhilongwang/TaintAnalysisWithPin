#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[]){
	int i = 1;
    while(i < argc)
    {
        printf("%s\n",argv[i]);
        i++;
    }
	char username[20];
	scanf("%s", username);
	if (strcmp(username,"zhilong") == 0){
		printf("pass\n");
	}else{
		printf("fail\n");
	}
	return 0;
}
