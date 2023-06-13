#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 1005

int system_stat;
int user_stat;
int count;

char input_txt[50];
char output_txt[50];
char line[MAX_LEN];
char word[MAX_LEN];

// 항목을 구분해서 해당 입력받은 문자로 9번 반복!
void divide_contents(char str[2000], FILE* out){
    for (int i = 0; i < 9; i++){
        // output_txt에 항목구분 하기 문자열 쓰기
        fputs(str, out);
        fputs("/", out);
        count++;
    }

    // 줄바꿈 5번
    for(int i=0; i<5; i++){
        fputs("\n", out);
    }
}

// 입력받은 문자열 9번 반복하는 함수
void repeat_9(char str[2000], FILE* out){
    // 문자열이 없을때 함수 종료
    int len = strlen(str);
    if (len == 0)
        return;

    // 9번 반복
    for (int i = 0; i < 9; i++){
        fputs(str, out);
        fputc('/', out);
        count++;
    }
    // 줄바꿈 5번
    for(int i=0; i<5; i++){
        fputs("\n", out);
    }
}

// 입력받은 문자열 9번 반복하는 함수 (DESCRIPTION 전용)
void repeat_9_decription(char str[2000], FILE* out){
    // 문자열이 없을때 함수 종료
    int len = strlen(str);
    if (len == 0)
        return;

    // 9번 반복
    for (int i = 0; i < 9; i++){
        fputc('/', out);
        fputs(str, out);
        for(int j=0; j<4; j++){
            fputs("\n", out);
        }
        count++;
    }
    /*
    // 줄바꿈 5번
    for(int i=0; i<5; i++){
        fputs("\n", out);
    }*/
}

// 앞에 데이터를 빼고 ": " 뒷부분인 유저 data만 문자열로 만드는 함수
int make_user_word(char string[MAX_LEN], char ret[MAX_LEN]) {
    char temp[MAX_LEN];
    int len = strlen(string);

    // 첫번째에 개행문자 '\n'가 있으면 데이터가 없다고 판단
    if (string[0] == 10) {
        // 문자가 없다는 뜻으로 배열의 첫번째에 '\0' 입력
        ret[0] = '\0';
        return 0; // 종료
    }

    // 문자열 개수만큼 반복문
    for (int i = 0; i < len; i++) {
        // 중간에 ": " 문자를 발견하면
        if (string[i] == ':') {
            // 앞부분을 전부 버리고, 유저정보만 골라서 ret 배열에 복사
            strcpy(ret, &string[i + 2]);
            int ret_len = strlen(ret);

            // 개행문자 삭제
            ret[ret_len - 1] = '\0';

            // 함수 종료
            return 1;
        }
    }
}

// 아이템 목록에 대한 정보를 문자열로 만드는 함수
int make_item_word(char string[2000], char ret[2000]){
    char temp[2000];
    int len = strlen(string);
    int j = 0;

    // 문자열이 없는 경우 함수 종료
    if (string[0] == 10){
        ret[0] = '\0';
        return 0;
    }

    // 문자열 길이만큼 반복
    for (int i = 0; i < len; i++){
        // ": " 문자만 빼고 ret에 값 그대로 저장
        // Ex) BOMB: 33 일경우 -> BOMB33 으로 문자열 가공
        if (string[i] != ':' && string[i] != ' '){
            // 개행문자 삭제
            if (string[i] == 10)
                ret[j++] = '\0';
            else
                ret[j++] = string[i];
        }
    }

    return 1;
}
// 그대로 ret배열에 복사.
int make_description_word(char string[2000], char ret[2000]) {
    strcpy(ret, string);
    return 1;
}
int main(int argc, char* argv[]) {
    // putty에서 ./encoder.out test1.txt encoded_data1 이런식으로 입력하면
    // argv[1] = test1.txt
    // argv[2] = endcoded_data1
    // 로 해당 배열에 값이 들어가게 된다.
    // 각각 input_txt, output_text 문자열로 저장
    strcpy(input_txt, argv[1]);
    strcpy(output_txt, argv[2]);
    printf("[input file]: %s\n", input_txt);
    printf("[output file]: %s\n", output_txt);

    // input_txt는 읽기 모드(mode: r)
    FILE* fp_input = fopen(input_txt, "r");
    if (fp_input == NULL) {
        printf("%s 을(를) 열 수 없습니다. 인코딩을 종료합니다.\n", input_txt);
        exit(0);
    }
    // output_txt는 쓰기 모드(mode: w)
    FILE* fp_output = fopen(output_txt, "w");
    if (fp_output == NULL) {
        printf("%s 을(를) 열 수 없습니다. 인코딩을 종료합니다.\n", output_txt);
        exit(0);
    }

    // input_txt에서 모든 라인을 한줄씩 읽어오기.
    while (fgets(line, sizeof(line), fp_input)) {
        // 리눅스에서 개행이 \r\n으로 잡히기 때문에 지우는 과정을 거쳐야함.
        // 한줄에 해당하는 문자열의 길이를 구한다음
        int len = strlen(line);
        for (int i = 0; i < len; i++) {
            // '\r'을 찾아서 지우는 동작
            // 아스키코드에서 '\r'값이 13
            if (line[i] == 13)
                strcpy(&line[i], strchr(line, 13) + 1);
            // strchr(str,문자): 문자가 포함된 곳의 포인터 반환
        }

        // 각 항목별로 구분하기 위한 stat변수 동작
        switch (system_stat) {
            case 0: // "*USER STATUS*\n" 를 만나면(개행문자까지 check)
                if (strcmp("*USER STATUS*\n", line) == 0) {
                    // 항목을 구분하는 함수
                    // U(User을 의미). U가 9번 반복
                    divide_contents("U", fp_output);
                    // 다음 1번으로 이동
                    system_stat = 1;
                }
                break;
            case 1:
                // "*ITEMS*\n" 를 만나면
                if (strcmp("*ITEMS*\n", line) == 0) {
                    // 항목을 구분하는 함수
                    // I(Items을 의미). I가 9번 반복
                    divide_contents("I", fp_output);
                    // 다음 2번으로 이동
                    system_stat = 2;
                }
                // 위의 조건이 아니라면
                else {
                    // 6개의 유저정보에 해당이 되면 아래 함수들 실행
                    if (user_stat <= 6) {
                        // 중간에 위치한 ": " 이후의 데이터만 추출
                        // 문자열을 자르는 함수 실행
                        make_user_word(line, word);
                        // 위에서 문자열을 9번 반복
                        repeat_9(word, fp_output);
                        // 유저정보는 6개이므로 6번만 동작하기 위함
                        user_stat++;
                    }
                }
                break;
            case 2:
                // "*FRIENDS LIST*\n" 를 만나면
                if (strcmp("*FRIENDS LIST*\n", line) == 0) {
                    // 항목을 구분하는 함수
                    // F(Items을 의미). F가 9번 반복
                    divide_contents("F", fp_output);
                    // 다음 3으로 이동
                    system_stat = 3;
                }
                else {
                    // 아이템 목록에 대한 문자열 가공
                    make_item_word(line, word);
                    // 위에서 문자열이 가공되서 나오면 9번 반복
                    repeat_9(word, fp_output);
                }
                break;
            case 3:
                // "*DESCRIPTION*\n" 를 만나면
                if (strcmp("*DESCRIPTION*\n", line) == 0) {
                    // 항목을 구분하기 위한 함수를 실행함. 이번엔 D로 9번 반복
                    divide_contents("D", fp_output);
                    // 다음 4로 이동
                    system_stat = 4;
                }
                else {
                    // 앞의 유저정보와 같은 함수
                    make_user_word(line, word);
                    // 위에서 문자열이 가공되서 나오면 9번 반복
                    repeat_9(word, fp_output);
                }
                break;
            case 4:
                // 설명 항목에 대한 문자열 가공
                make_description_word(line, word);
                // 위에서 문자열이 가공되서 나오면 9번 반복
                repeat_9_decription(word, fp_output);
                break;
        }
    }

    // 인코딩 완료!
    printf("\ncomplete encoding!\n");

    return 0;
}
