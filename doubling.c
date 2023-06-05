#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h> 
#include <stdlib.h>

int check_params(int argc, char* argv[]) {
	if (argc == 3)
		return 1;
	return 0;
}

int main(int argc, char* argv[]) {
	if(check_params(argc, argv) == 0) {
		printf("Usage: %s should have 2 parameter.\n", argv[0]);
		exit(1);	
	}

	FILE* fp = fopen(argv[1], "r");
	FILE* encode = fopen(argv[2], "wt");
	FILE* fp2 = fopen("double_test.txt", "r");	

	if (fp ==NULL)
		printf("failed to open file");
	if (fp2 == NULL)
		printf("failed to open file");
 
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
