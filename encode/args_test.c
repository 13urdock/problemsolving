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
	FILE* fp2 = fopen(argv[2], "wt");
	
	if (fp ==NULL)
		printf("failed to open file");
	if (fp2 == NULL)
		printf("failed to open file");
		
	return 0;
}
