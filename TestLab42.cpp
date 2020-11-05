#include<stdio.h>
#include<string.h>

char input[100];
char str[100];
char str2[100];
char path[100];
int i = 0;


int main(){
	FILE *fp;
	int stage = 0;
	short exit = 0;
	scanf("%s",input);
	while(!exit){
		if(stage == 0){
			if(input[i] != '*'){
				str[i] = input[i];
				i++;
			}
			else{
				i = 0;
				int j = strlen(str);
				while(str[j - 1 ] != '\\'){
					str2[i] = str[j - 1];
					i++;
					j--;
				}
				printf("Sender Send : ");
				for(i = strlen(str2) - 1;i >= 0;i--)
					printf("%c",str2[i]);
				stage++;
			}
		}
		else if(stage == 1){
			printf("\nSave as : ");
			scanf("%s",path);
			fp = fopen(path,"a+");
			fclose(fp);
			stage++;
			exit = 1;
		}
	}
	printf("%s",input);
	fclose(fp);
}
