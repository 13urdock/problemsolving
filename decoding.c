#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input_txt[50];
char output_txt[50];

char line[1005];
char str_buf[1001][1001]; // 1줄에 1글자씩 1000자 || 1줄에 1000자
char desc_buf[1001][1001]; // 1줄에 1글자씩 1000자 || 1줄에 1000자

int str_arr_num = 0; // 일반 상태 문자열 번호
int desc_arr_num = 0; // desc 상태 문자열 번호
int system_stat = 0; // system 상태, switch문 case

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
void friend_info(char* friend[]){ // 친구 상태 초기화
    friend[0] = "ID";
    friend[1] = "NAME";
    friend[2] = "GENDER";
    friend[3] = "AGE";
}
int is_equal(char str1[1001], char str2[1002]){
    if( strcmp(str1, str2) == 0 )
        return 1;
    return 0;
}
int include_slash(char str[2000]){
    int slash_count = 0;
    int len = strlen(str);
    for(int i=0; i<len; i++){
        if( str[i] == '/' )
            slash_count++; // 슬래시 수 세기
    }
    return slash_count;
}
void add_str_arr(char str[2000]){ // str_buf에 문자열 추가
    int i=0;
    char *token;
    int len = strlen(str);
    //printf("%s\n", str);
    str[len-1] = '\0'; // 맨끝 \n 삭제

    token = strtok(str, "/\n"); // 토큰 단위로
    while( (token != NULL) && (i < 9) ){
        strcpy(str_buf[i],token);
        token = strtok(NULL, "/");
        i++;
    }
    /*
    for(int i=0; i<9; i++)
        printf("%d: %s\n",i,str_buf[i]);

    printf("\n");
    */
}
int check_origin_data(char str_buf[][1001]){ // str_buf에 추가된 문자 다수결로 출력
    int count[9] = {0,}; // str_buf가 다른 문자열과 일치하는지 count하는 배열

    int max = 0;
    int index = 0;

    for(int i=0; i<9; i++){
        for(int j=i+1; j < 9; j++){
            if( is_equal( str_buf[i], str_buf[j] )){
                count[i]=count[i]+1;
            }
        }
    }
    /*
    for(int i=0; i<9; i++){
        printf("%d ",count[i]);
    }
    printf("\n");
    */
    // 가장 많이 나온 값의 index 리턴
    for(int i=0; i<9; i++){
        if( count[i] > max ){
            max = count[i];
            index = i;
        }
    }
    return index;
}
int separate(char* str, char* nums, char* strs){ // 문자열과 숫자 구분
    int num_index=0;
    int str_index=0;

    for(int i=0; i<strlen(str); i++){ // 문자 다 검사
        if( str[i] >= '0' && str[i] <='9' ){ // 문자가 0-9까지면
            nums[num_index++]=str[i];
        }
        else{
            strs[str_index++]=str[i];
        }
    }
    nums[num_index] = '\0'; // 숫자 문자열의 끝
    strs[str_index] = '\0'; // 문자 문자열의 끝
}
void item_status(Item *item,char str[2000]){
    // BOMB1을
    // 문자열 2개에 BOMB와 1담아서 구조체에 저장
    //printf("%s\n",str);
    int len = strlen(str);
    char *nums = (char*)malloc((len+1)* sizeof(char));
    char *strs = (char*)malloc((len+1)* sizeof(char));

    separate(str, nums, strs); // 문자열과 숫자 분리

    //printf("strs: %s\n",strs);
    //printf("nums: %s\n",nums);

    strcpy(item->name,strs); // 구조체에 옮겨담기
    strcpy(item->data,nums);

    free(nums);
    free(strs);
}
void shift_left(char str_buf[][1001]){
    int len = 0;

    for(int i = 0; i < 9; i++){
        len = strlen(str_buf[i]); // 문자열의 길이

        for(int j = 1; j < len; j++){ // '/'든 변형된 문자든 한글자씩 앞으로 shift
            str_buf[i][j-1] = str_buf[i][j];
        }
        str_buf[i][len-2] = '\0'; // 문자열의 끝의 개행문자를 '\0'으로
    }
//  for(int i=0; i<9; i++)
//      printf("%s\n",str_buf[i]);
}
void add_desc_arr(char str[2000]){
    strcpy(desc_buf[desc_arr_num],str); // 버퍼에 문자열 저장
    /*for(int i=0; i<9; i++)
        printf("[%d]: %s\n",i,desc_buf[i]);
    */
    desc_arr_num = (desc_arr_num+1) % 9; // 0-8번
}
void run(FILE *read, FILE *write){
    char *user[2000]; // 유저 상태 기록
    Item *item = (Item*)malloc(sizeof(Item)); // 아이템 상태 기록
    char *friend[2000]; // 친구 상태 기록

    char conversion[2000]; // 변환된 문자열
    char description[1001]; // desc 문자열

    int user_menu = 0; // 유저 메뉴 수
    int friend_num = 0; // 친구 수
    int friend_menu = 0; // 친구 메뉴 수

    int num = 0; // 검사 후 나온 올바른 배열 번호
    int flag = 0;
    if (read == NULL || write == NULL){
        printf("파일을 열 수 없습니다.\n");
        exit(0);
    }
    user_info(user); // 유저 상태 배열에 기록 (ID: NAME: 등)
    friend_info(friend); // 친구 상태 배열에 기록 (ID: NAME: 등)

    while (fgets(line, sizeof(line), read)){ // 한줄을 리턴
        if( is_equal(line,"\n") ){ // "\n"만 있다면
            continue;
        }
        if( is_equal(line,"/\n") ){ // "/"만 있다면
            continue;
        }
        if(flag == 0){ // flag가 1이 되면 desc배열에 저장
            if( include_slash(line) < 5 ){ // 슬래시가 최소 5번이 없다면
                continue;
            }
            add_str_arr(line); // 문자열 추가
            num = check_origin_data(str_buf); // 원본 데이터를 찾는 인덱스
            strcpy(conversion, str_buf[num]); // 변환된 문자열 옮겨담기
        }
        switch(system_stat){
            case 0:
                if( is_equal(conversion, "U") ){ // 문자열이 "U(User)"라면
                    fprintf(write,"*USER STATUS*\n"); // 파일에 입력
                    system_stat = 1; // 다음 case로
                }
                break;
            case 1:
                if( is_equal(conversion, "I") ){ // 문자열이 "I(Item)"라면
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
                if( is_equal(conversion, "F") ){ // 문자열이 "F(Friends)"라면
                    fprintf(write,"\n*FRIENDS LIST*\n");
                    system_stat = 3; // 다음 case로
                }
                else{
                    item_status(item,conversion);
                    fprintf(write,"%s: %s\n",item->name, item->data); // 파일에 입력
                }
                break;
            case 3:
                if( is_equal(conversion, "D") ){ // 문자열이 "D(Description)"라면
                    fprintf(write,"*DESCRIPTION*\n");
                    system_stat = 4; // 다음 케이스로
                    flag = 1; // desc_arr에 저장
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
                add_desc_arr(line); // desc문자열 배열에 문자열 저장
                if( desc_arr_num == 0 ){ // 9번,즉 다시 0번이 되면
                    shift_left(desc_buf); // str_buf 한칸씩 왼쪽으로 밀기
                    num = check_origin_data(desc_buf);  // 원본 데이터를 찾는 인덱스
                    fprintf(write,"%s\n", desc_buf[num]); // 원본 데이터 파일에 입력
                    //printf("%s\n",desc_buf[num]);문자열을 ‘/’단위로 잘라서
                    memset(desc_buf, 0, sizeof(desc_buf)); // 배열 초기화
                }
                break;
        }
    }
    free(item);
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

    run(fp_input,fp_output); // 함수 실행

    fclose(fp_input);
    fclose(fp_output);

    return 0;
}
