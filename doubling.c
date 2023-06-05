#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h> 

int main(int argc, char* argv[]){ 
	FILE* fp = fopen("double_test.txt", "r");
	FILE* encode = fopen("encoded_text.txt", "wt"); 
	FILE* fp2 = fopen("double_test.txt", "r");	
	char read, read2;

	do{
		fscanf(fp, "%c", &read);

		//fp 읽고 출력
		if (read == ' ' || read == '\n'){//공백이나 엔터를 만나면 슬래시 치고 fp2 스캔 시작	
			fprintf(encode, "/");

			//fp2 읽고 출력
			do{
				fscanf(fp2, "%c", &read2); //scan first
				if (read2 == ' ' || read2 == '\n'){//공백이나 엔터를 만나면
					fprintf(encode, "/");				//슬래시 출력
					break;
				}
				else{
					fprintf(encode, "%c", read2); 	//or 글자 출력
				}
			}while(read2 != ' '|| read2 !='\n');
		}
		else{
			fprintf(encode, "%c",read);
		}
	}while (!feof(fp));

	fclose(fp);
	fclose(encode);

	return 0;
}
