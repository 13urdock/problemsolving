#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char input_txt[50];
char output_txt[50];
char buffer[10][256];

char compare[256];
int count = 0;

typedef struct{
    char id[256];
    char name[256];
    char gender[7]; // FEMALE, MALE
    char age[3]; // 0-99
    char hp[4]; // 0-255
    char mp[4]; // 0-255
    char coin[6]; // 0-65535
}User;
typedef struct{
    char name[256]; // 무기 이름
    char data[256]; // 무기 개수
}Item;
// '/'문자 이전의 문자열 return
// 9개 반복되므로 9번 scan
char* separation(FILE *read){
    for(int i=0; i<9; i++)
        fscanf(read, "%[^\n//]//",buffer[i]);
    /*
        문자열 9개를 비교해서
        가장 많이 나온 문자가 올바른 문자이므로
        그 문자를 리턴하는 코드
    */
    return buffer[8];
}
void sep_item(Item *item,FILE *read,FILE *write){

}
Item item_status(Item *item, FILE *read,FILE *write){
    // 구조체 item에는
    // 무기 이름과 무기 개수가 포함되어있다.
    for(int i=0; i<9; i++){
        fscanf(read, "%[^\n//]//",item[i].name); // 무기 이름
        if( strcmp(item[i].name, "F") == 0 ){ // 이름이 같으면
            fprintf(write,"\n*FRIENDS LIST*\n");
            /*

            */
            count=6;
            return item[8];
        }

        fscanf(read, "%[^\n//]//",item[i].data); // 무기 개수

    }
    /*
        아이템 문자열 9개와 아이템 숫자 9를 비교해서
        가장 많이 나온 문자가 올바른 문자이므로
        그 구조체를 리턴하는 코드
    */
    return item[8];
}
void save_item_status(Item *item, FILE *write){
    fprintf(write, "%s: %s\n",item->name,item->data);
}
void user_status(User *user, FILE *read){
    strcpy(user->id,separation(read));
    strcpy(user->name,separation(read));
    strcpy(user->gender,separation(read));
    strcpy(user->age,separation(read));
    strcpy(user->hp,separation(read));
    strcpy(user->mp,separation(read));
    strcpy(user->coin,separation(read));
}
void save_user_status(User *user, FILE *write){
    fprintf(write, "ID: %s\n",user->id);
    fprintf(write, "NAME: %s\n",user->name);
    fprintf(write, "GENDER: %s\n",user->gender);
    fprintf(write, "AGE: %s\n",user->age);
    fprintf(write, "HP: %s\n",user->hp);
    fprintf(write, "MP: %s\n",user->mp);
    fprintf(write, "COIN: %s\n",user->coin);
}
void run(FILE *read, FILE *write){
    User user;
    Item item[9];
    Item i;
    if (read == NULL || write == NULL){
        printf("파일을 열 수 없습니다.\n");
        exit(0);
    }
    //fscanf(fp, "%[^\n]//",buffer);

    while(1){
        strcpy(compare,separation(read));
        printf("%s\n",compare);
        if( strcmp(compare, "U") == 0 ){
            fprintf(write,"*USER STATUS*\n");

            user_status(&user,read); // 유저 정보 저장
            save_user_status(&user,write); // 파일에 유저 정보 저장
            continue;
        }
        else if( strcmp(compare, "I") == 0 ){
            fprintf(write,"\n*ITEMS*\n");

            while( count < 4){
                i = item_status(item,read,write);
                save_item_status(item,write);
                count++;
            }
            continue;
        }
        else if( strcmp(compare, "F") == 0 || count == 6){
            fprintf(write,"\n*FRIENDS LIST*\n");
            break;
        }
        else if( strcmp(compare, "D") == 0 ){
            fprintf(write,"\n*DESCRIPTION*\n");
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
