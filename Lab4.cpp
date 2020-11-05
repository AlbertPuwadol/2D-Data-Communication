#include<stdio.h>
#include<string.h>

int main(){
	FILE *fp;
	int ack = 0;
	char buffer[10] = {};
	char str[100];
	char path[100];
	int stage = 0,i = 0;
	while(true){
		char frame = get_character();
		if(stage == 0){
			if(frame != '*'){
				str[i] = frame;
				i++;
			}
			else{
				int size = strlen(str);
				char str2[100];
				int j = 0;
				while(str(size - 1) != '\\'){
					str2[j] = str(size - 1);
					j++;
					size--;
				}
				printf("Sender Send : ");
				for(int k = strlen(str2) ; k > 0 ; k--)
					printf("%c",str2[ k - 1 ]);
				stage++;
			}
		}
		else if(stage == 1){
			printf("Save as : ");
			gets(path);
			fopen(path,"a+");
			stage++;
		}
		else if(stage == 2){
			if(frame == '0' || frame == '1'){
				printf("Receive frame : ");
				printf("%c",frame);
				stage++;
				str = {};
			}
		}
		else if(stage == 3){
			if(frame == '*'){
				stage++;
			}
		}
		else if(stage == 3){
			if(frame == '*')
				printf("Data       : ");
				stage = 2;
			else
				fputc(frame,fp);
		}
	
	}
	
	
	fclose(fp);
}
