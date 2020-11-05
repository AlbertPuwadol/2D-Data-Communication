#include<stdio.h>
#include<string.h>

char input[100];
char str[100];
char str2[100];
char path[100];
int i = 0;

int main(){
	FILE *fp;
	scanf("%s",input);
	while(input[i] != '*'){
		str[i] = input[i];
		i++;
	}
	i = 0;
	int j = strlen(str);
	while(str[j - 1 ] != '\\'){
		str2[i] = str[j - 1];
		i++;
		j--;
	}
	for(i = strlen(str2) - 1;i >= 0;i--)
		printf("%c",str2[i]);
	
	printf("\nSave as : ");
	scanf("%s",path);
	fp = fopen(path,"a+");
	fclose(fp);
	
}
	
