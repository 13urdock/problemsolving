#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input_txt[50];
char output_txt[50];

char line[1005];
char buffer[12][2000];
char str_buf[1001][1001]; // 1줄에 1글자씩 1000자 || 1줄에 1000자

int str_arr_num = 0;
int system_stat = 0;

typedef struct{
    char name[256]; // 무기 이름
    char data[256]; // 무기 개수
}Item;

void user_info(char* user[]){ // 유저 상태 초기화
    user[0] = "ID";
    user[1] = "NAME";
    user[2] = "GENDER";
     user[3] = "AGE";
     user[4] = "HP";
     user[5] = "MP";
     user[6] = "COIN";
 }
 void friend_info(char* friend[]){ // 유저 상태 초기화
     friend[0] = "ID";
     friend[1] = "NAME";
     friend[2] = "GENDER";
     friend[3] = "AGE";
 }
 char* separate(char str[2000]){
     for(int i=0; i<9; i++)
         sscanf(str, "%[^\n/]/",buffer[i]);
     //for(int i=0; i<12; i++)
     //  printf("%s",buffer[i]);
     //printf("\n");
     /*
         문자열 9개를 비교해서
         가장 많이 나온 문자가 올바른 문자이므로
         그 문자를 리턴하는 코드
     */
     //printf("%ld\n",strlen(str));
     return buffer[8];
 }
 void item_status(Item *item,char str[2000]){
      char token1[10];
      char token2[10];
      int index = 0;

			int error_count = 0; // 아이템 구조체의 오류 개수
     	char error_str[256]; // 오류 발생 시 반복되는 문자열
     	char error_data[256]; // 오류 발생 시 반복되는 데이터

			char str_buffer[9][256];
     	int str_count = 0; // 현재까지 읽은 문자열의 개수

      int i=0;

			while( sscanf(str+i, "%[^/]/%[^/]/",item[index].name,item[index].data) == 2 ){
         i += strlen(token1) + strlen(token2) + 2; // 다음 토큰의 시작 위치, / 두개이므로 +2
         index++;
      }
  
      if (error_count == 0 && str_count < 9) {
     		  // 9개의 문자열을 저장
          strcpy(str_buffer[str_count], item->name);
          str_count++;
			}
		 	else if (error_count == 0 && str_count == 9) {
					// 9개의 문자열을 모두 읽은 경우
					int  same_count = 0; // 반복되는 문자열 개수
					for (int i = 1; i < 9; i++) {
         		if (strcmp(str_buffer[i], str_buffer[i-1]) == 0) {
              same_count++;
         		}  
						else 
              same_count = 1;
           	if (same_count >= 6) {
              // 오류 복구
              strcpy(error_str, str_buffer[i]);
              strcpy(error_data, item->data);
              error_count++;
              break;
            }
         	}
					// '\n' 문자에서 오류가 발생했을 때의 처리를 추가
          if (strchr(item->name, '\n') || strchr(item->data, '\n')) {
          	// 오류 개수 증가수 
						error_count++;
					}	
          // 첫 번째 문자열과 다른 경우
          if (strcmp(str_buffer[0], item->name) != 0) {
            strcpy(str_buffer[0], str_buffer[8]);
            str_count = 1;
          }
     	}
 }
void shift_left(){
    int len = 0;
    for(int i = 0; i < 9; i++){
        len = strlen(str_buf[i]); // 문자열의 길이
        str_buf[i][len-1] = '\0'; // 문자열의 끝의 개행문자를 '\0'으로
        for(int j = 1; j < len; j++){ // '/'든 변형된 문자든 한글자씩 앞으로 shift
            str_buf[i][j-1] = str_buf[i][j];
        }
    }
//  for(int i=0; i<9; i++)
//      printf("%s\n",str_buf[i]);
}
int is_equal(char str1[1001], char str2[1002]){
    if( strcmp(str1, str2) == 0 )
        return 1;
    return 0;
}
int check_desc_origin_data(){
    int count = 0; // 문자열이 같은 배열이 있는 수
    for(int i=0; i<9; i++){
        for(int j=0; j < 9; j++){
            if( is_equal( str_buf[i], str_buf[j] )){
                count++;
            }
        }
        if( count >= 5 ) // 5번 이상 같으면
            return i; // i인덱스 출력
        count = 0; // count 초기화
    }
    return -1; // 잘못 출력 시
}
void add_str_arr(char str[2000]){
    strcpy(str_buf[str_arr_num],str); // 버퍼에 문자열 저장
    str_arr_num = (str_arr_num+1) % 9;
}
 void run(FILE *read, FILE *write){
     char *user[2000]; // 유저 상태 기록
     Item item[9]; // 아이템 상태 기록
     char *friend[2000]; // 친구 상태 기록
     char conversion[2000]; // 변환된 문자열
     char description[1001]; // desc 문자열
     int user_menu = 0; // 유저 메뉴 수
     int friend_num = 0; // 친구 수
     int friend_menu = 0; // 친구 메뉴 수
     int num = 0;
     int error_count = 0; // 아이템 구조체의 오류 개수
     char error_str[256]; // 오류 발생 시 반복되는 문자열
     char error_data[256]; // 오류 발생 시 반복되는 데이터
     if (read == NULL || write == NULL){
         printf("파일을 열 수 없습니다.\n");
         exit(0);
     }
     user_info(user); // 유저 상태 배열에 기록 (ID: NAME: 등)
     friend_info(friend); // 친구 상태 배열에 기록 (ID: NAME: 등)
      // 9개의 문자열을 저장하기 위한 배열
     char str_buffer[9][256];
     int str_count = 0; // 현재까지 읽은 문자열의 개수
     while (fgets(line, sizeof(line), read)){ // 한줄을 리턴
         //printf("%s",line);
         strcpy(conversion, separate(line)); // 변환된 문자열 옮겨담기
         switch(system_stat){
             case 0:
                 if( strcmp(conversion, "U") == 0 ){ // 문자열이 "U(User)"라면
                     fprintf(write,"*USER STATUS*\n"); // 파일에 입력
                     system_stat = 1; // 다음 case로
                 }
                 break;
             case 1:
                 if( strcmp(conversion, "I") == 0 ){ // 문자열이 "I(Item)"라면
                     fprintf(write,"\n*ITEMS*\n");
                     system_stat = 2; // 다음 case로
                 }
                 else{
                     if(user_menu < 7){
                         fprintf(write,"%s: %s\n",user[user_menu],conversion); // 파일에 입력
                         user_menu++; // 메뉴 개수만큼
                     }
                 }
                 break;
             case 2:
                 if( strcmp(conversion, "F") == 0 ){ // 문자열이 "F(Friends)"라면
                     fprintf(write,"\n*FRIENDS LIST*\n");
                     system_stat = 3; // 다음 case로
                 }
                  else{
                     item_status(item,line);
                     fprintf(write,"%s: %s\n",item->name, item->data); // 파일에 입력 
                 }
                 break;
             case 3:
                 if( strcmp(conversion, "D") == 0 ){ // 문자열이 "D(Description)"라면
                     fprintf(write,"*DESCRIPTION*\n");
                     system_stat = 4; // 다음 케이스로
                 }
                 else{
                     fprintf(write,"FRIEND%d %s: %s\n",friend_num+1,friend[friend_menu],conversion);
                     friend_menu = (friend_menu + 1) % 4;
                     if(friend_menu == 0){ // 메뉴가 4번 파일에 입력되면
                         friend_num++; // 친구 수 증가
                         fprintf(write,"\n"); // 줄바꿈
                     }
                 }
                 break;
             case 4:
                 add_str_arr(line); // 문자열 배열에 문자열 저장 및 오류 검출
                 //printf("%s",str_buf[0]);
                 if( str_arr_num == 8 ) // 8번이 되면
                     fprintf(write,"%s", str_buf[num]); // num은 본인이 지정
                 break;
         }
     }
 }
 int main(int argc, char* argv[]){
     // putty에서 ./decoder.out encoded_data1.modified result1.txt 이런식으로 입력하면
     // argv[1] = encoded_data1.modified
     // argv[2] = result1.txt
     // 로 해당 배열에 값이 들어가게 된다.
     // 각각 input_txt, output_text 문자열로 저장
     strcpy(input_txt, argv[1]);
     strcpy(output_txt, argv[2]);
     printf("[input file]: %s\n", input_txt);
     printf("[output file]: %s\n", output_txt);
     // input_txt는 읽기 모드(mode: r)
     FILE* fp_input = fopen(input_txt, "r");
     if (fp_input == NULL){
         printf("%s 을(를) 열 수 없습니다. 디코딩을 종료합니다.\n", input_txt);
         exit(0);
     }
     // output_txt는 쓰기 모드(mode: w)
     FILE* fp_output = fopen(output_txt, "w");
     if (fp_output == NULL){
         printf("%s 을(를) 열 수 없습니다. 디코딩을 종료합니다.\n", output_txt);
         exit(0);
     }
     run(fp_input,fp_output);
     fclose(fp_input);
     fclose(fp_output);
     return 0;
 }
