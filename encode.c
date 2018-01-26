#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 81

typedef struct _keitaiso {
    char hinshi[30];
    char word[200];
} keitaiso;

typedef struct _relation {
    char jap[4];
    char gro[12];
} relation;

keitaiso keitaisos[100];
relation japgro[N];

char ya[3] = "ャ";
char yu[3] = "ュ";
char yo[3] = "ョ";
char xa[3] = "ァ";
char xi[3] = "ィ";
char xe[3] = "ェ";
char xo[3] = "ォ";
char nobi[3] = "ー";
char kitsu[3] = "ッ";
char ki[3] = "キ";
char ti[3] = "チ";
char zi[3] = "ジ";
char fu[3] = "フ";


//コマンドライン引数の文章を形態素に分解してsentence.txtに入れておく
void read_sentence(char *arg_setence) {
    char a[100] = "bash use_mecab.sh ";
    strcat(a, arg_setence);
    system(a);
}

//sentence.txtからデータを引っ張ってきて、keitaisosに入れ直す
int write_sentence(char *filename) {
    int i = 0;
    char text[10][200];
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        return EXIT_FAILURE;
    }

    while (fscanf(fp, "%s\t%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",text[0],text[1],text[2],text[3],text[4],text[5],text[6],text[7],text[8],text[9]) != EOF){
        printf("%s %s %s %s %s %s %s %s %s %s \n", text[0],text[1],text[2],text[3],text[4],text[5],text[6],text[7],text[8],text[9]);
        if(strcmp(text[0], "EOS")==0) {
            break;
        }
        else {
            strcpy(keitaisos[i].hinshi, text[1]);
            if(strcmp(text[8], "")==0) {
                strcpy(keitaisos[i].word, text[0]);
            }
            else {
                strcpy(keitaisos[i].word, text[8]);
            }
        }
        i++;
    }

    for(int j=0; j<i; j++) {
        printf("this is %d'st word\n", j);
        printf("%s %s\n", keitaisos[j].hinshi, keitaisos[j].word);
    }

    return i;
}

//grongi_rule.txtから対応関係を引っ張ってきてjapgroに入れる
void grongi_init() {
    FILE *fp;
    char *filename = "grongi_rule.txt";
    int i=0;

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        exit(0);
    }

    while(fscanf(fp, "%s %s",japgro[i].jap, japgro[i].gro)!=EOF) {
        printf("%s %s\n", japgro[i].jap, japgro[i].gro);
        i++;
    }
}

//今の文字と後ろの文字を見て、例外処理をする
/*char *excep_hand(char c1, char c2, char c3, char c4, char c5, char c6, int j)  {
    char result[6] = "";
    //っが出てきたら後ろの文字を重ねる処理
    if(c1==kitsu[0] && c2 == kitsu[1] && c3 == kitsu[2]) {
        result[0] = c4;
        result[1] = c5; 
        result[2] = c6;
        result[3] = c4;
        result[4] = c5;
        result[5] = c6;
    }
    else if()
}*/

void translate(int num_word, keitaiso *keitaisos) {
    char *word = malloc(sizeof(char)*100);
    int word_len = 0;
    int word_count = 0;
    int flag = 0;
    printf("%d\n", num_word);
    char result_sentence[300] = "";
        
    for(int i=0; i<num_word; i++) {
        word_len = 0;
        word_count=0;
        char result_word[300] = "";
        strcpy(word, keitaisos[i].word);
        word_len = strlen(word);
        printf("%s %d\n", word, word_len);
        while(word_count<word_len) {
            flag = 0;
            for(int j=0; j<N; j++) {
                if(japgro[j].jap[0]==word[word_count] && japgro[j].jap[1]==word[word_count+1] && japgro[j].jap[2]==word[word_count+2]) {
                    //printf("%s %s\n", japgro[j].jap, japgro[j].gro);
                    strcat(result_word, japgro[j].gro);
                    flag = 1;
                    break;
                }
            }
            if(flag==0) {
                printf("N");
            }
            word_count += 3;
        }
        strcat(result_sentence, result_word);
        printf("word_count = %d\n", word_count);
    }
    printf("result is %s\n", result_sentence);
}

int main(int argc, char **argv) {
    char *filename = "sentence.txt";

    grongi_init();
    
    read_sentence(argv[1]);

    int num_word = write_sentence(filename);

    translate(num_word, keitaisos);

    return 0;
}