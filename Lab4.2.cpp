#include<stdio.h>
#include<string.h>

void append(char* s, char c) {
	int len = strlen(s);
	s[len] = c;
	s[len+1] = '\0';
}

int input;
char str[100];
char str2[100];
char path[100];
int i, j;

int main(){
	FILE *fp;
	char buffer[8];
	int stage = 0;
	short endOfFile = 0;
	short exit = 0;
	char frame;
	char checkFrame = '0';
	char ack;
	
	while(!exit){
		input = get_character();		
		if(stage == 0){
			if(input != '*'){
				str[i] = (char)input;
				i++;
			}
			else{
				i = 0;
				j = strlen(str);
				while(str[j - 1] != '\\'){
					str2[i] = str[j - 1];
					i++;
					j--;
				}
				printf("Sender Send : ");
				for(i = strlen(str2) - 1 ; i >= 0 ; i--)
					printf("%c",str2[i]);
				stage++;	
			}
		}
		else if(stage == 1){
			printf("\nSave as : ");
			scanf("%s",path);
			fp = fopen(path,"a+");
			stage++;
		}
		else if(stage == 2){
			if(input == '0' || input == '1'){
				frame = (char)input;
				printf("Receive frame			 : %c\n",frame);
				stage++;
				for(i = 0 ; i < 8 ; i++){
					buffer[i] = '\0';
				}
			}
		}
		else if(stage == 3){
			if(input == '*'){
				stage++;
			}
		}
		else if(stage == 4){
			if(input == '*'){
				printf("Data       : %s\n",buffer);
				scanf("%c",&ack);
				if(ack == 'a'){
					if(frame == checkFrame){
						if(checkFrame == '0'){
							checkFrame = '1';
							for(i = 0 ; i < 8 ; i++){
								if(buffer[i] == EOF || endOfFile){
									endOfFile = 1;
									exit = 1;
								}
								else
									fputc(buffer[i],fp);
							}	
						}
						else if(checkFrame == '1'){
							checkFrame = '0';
							for(i = 0 ; i < 8 ; i++){
								if(buffer[i] == EOF || endOfFile){
									endOfFile = 1;
									exit = 1;
								}
								else
									fputc(buffer[i],fp);
							}			
						}
					}
					else{
						printf("Reject & Send ACK%c\n\n",checkFrame);	
					}
					if(!exit){
						printf("Received & Send ACK%c\n\n",checkFrame);
						send_character(checkFrame);
					}
					else{
						printf("Received & Send ACK\n\n");
						send_character('e');
					}
					
				}
				else if(ack == 'r'){
					printf("Reject & Sleep\n\n");
					send_character('t');
				}
				else if(ack == 'n'){
					if(frame == checkFrame){
						if(checkFrame == '0'){
							checkFrame = '1';
							printf("Received & Sleep\n\n");
							for(i = 0 ; i < 8 ; i++){
								fputc(buffer[i],fp);	
							}	
						}
						else if(checkFrame == '1'){
							checkFrame = '0';
							printf("Received & Sleep\n\n");
							for(i = 0 ; i < 8 ; i++){
								fputc(buffer[i],fp);
							}
		
						}
					}
					else{
						printf("Reject & Sleep\n\n");
					}
					send_character('t');
				}
				stage = 2;
			}
			else
				append(buffer,(char)input);
		}
	}
	fclose(fp);
}
