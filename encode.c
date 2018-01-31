#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 81

typedef struct _keitaiso {
    char hinshi[30];
    char hinshi2[30];
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
char ga[3] = "ガ";
char no[3] = "ノ";
char ha[3] = "ハ";
char wo[3] = "ヲ";
char de[3] = "デ";
char to[3] = "ト";


//コマンドライン引数の文章を形態素に分解してsentence.txtに入れておく
void read_sentence(char *arg_setence) {
    char a[100] = "bash use_mecab.sh ";
    strcat(a, arg_setence);
    system(a);
}

//sentence.txtからデータを引っ張ってきて、keitaisosに入れ直す
int write_sentence(char *filename) {
    int i = 0;
    int count = 0;
    char readline[2000] = {'\0'};
    char text[10][200];
    char *p;
    FILE *fp;

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        return EXIT_FAILURE;
    }

    while (fgets(readline, 2000, fp) != NULL){
        count = 0;

        for(int i=0; i<10; i++) {
            strcpy(text[i], "undef");
        }

        p = strtok(readline, "\t,");
        while(p) {
            strcpy(text[count], p);
            p = strtok(NULL, "\t,");
            count++;
        }
        if(strcmp(text[0], "EOS")==0) {
            break;
        }
        else {
            strcpy(keitaisos[i].hinshi, text[1]);
            //printf("品詞は%sです\n", text[1]);
            strcpy(keitaisos[i].hinshi2, text[2]);
            //printf("品詞2は%sです\n", text[2]);
            if(strcmp(text[8], "undef")==0) {
                //printf("%s\n", text[0]);
                strcpy(keitaisos[i].word, text[0]);
            }
            else {
                //printf("%s\n", text[8]);
                strcpy(keitaisos[i].word, text[8]);
            }
        }
        i++;
    }

    for(int j=0; j<i; j++) {
        //printf("this is %d'st word\n", j);
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
        //printf("%s %s\n", japgro[i].jap, japgro[i].gro);
        i++;
    }
}

//今の文字と後ろの文字を見て、例外処理をする
char *excep_hand(char c1, char c2, char c3, char c4, char c5, char c6, int ii, int j, int word_count, char *word, char *result_word)  {
    char *result = malloc(sizeof(char)*10);
    if(word_count==0) {
        //printf("reach first\n");
        if(strcmp(keitaisos[ii].hinshi, "助詞")==0) {
            //printf("reach joshi\n");
            if(c4==ga[0] && c5 == ga[1] && c6 == ga[2]) {
                return "グ";
            }
            if(c4==no[0] && c5 == no[1] && c6 == no[2]) {
                return "ン";
            }
            if(c4==ha[0] && c5 == ha[1] && c6 == ha[2]) {
                return "パ";
            }
            if(c4==wo[0] && c5 == wo[1] && c6 == wo[2]) {
                return "ゾ";
            }
            if(c4==de[0] && c5 == de[1] && c6 == de[2]) {
                return "グ";
            }
            if(c4==to[0] && c5 == to[1] && c6 == to[2]) {
                return "ド";
            }
            else {
                return japgro[j].gro;
            }
        }
        else {
            return japgro[j].gro;
        }
    }
    //もしフが出てきたら何も連結せずに返す
    if(c4==fu[0] && c5==fu[1] && c6==fu[2]) {
        return "";
    }
    //もしッが出てきたら何も連結せずに返す
    if(c4==kitsu[0] && c5==kitsu[1] && c6==kitsu[2]) {
        return "";
    }
    //ッの後ろの文字を重ねる例外処理
    if(c1==kitsu[0] && c2==kitsu[1] && c3==kitsu[2]) {
        for(int i=0; i<N; i++) {
            if(japgro[i].jap[0]==word[word_count] && japgro[i].jap[1]==word[word_count+1] && japgro[i].jap[2]==word[word_count+2]) {
                //printf("%s", japgro[i].jap);
                strcat(result, japgro[i].gro);
                strcat(result, japgro[i].gro);
            }
        }
        //printf("kitsu\n");
        return result;
    }
    //ーが出てきたら前の文字を重ねる処理
    if(c4==nobi[0] && c5 == nobi[1] && c6 == nobi[2]) {
        //フだけその中でさらに例外、result_wordを参照できないので
        if(c1==fu[0] && c2==fu[1] && c3==fu[2]) {
            return "ズズ";
        }
        else {
            for(int i=0; i<N; i++) {
                if(japgro[i].jap[0]==word[word_count-3] && japgro[i].jap[1]==word[word_count-2] && japgro[i].jap[2]==word[word_count-1]) {
                    return japgro[i].gro;
                }
            }
        }
    }
    //ファ、フィ、フェ、フォが出てきたときの例外処理
    else if (c4==xa[0] && c5 == xa[1] && c6 == xa[2]) {
        if(c1==fu[0] && c2==fu[1] && c3==fu[2]) {
            //printf("reach 4");
            return "ザ";
        }
    }
    else if (c4==xi[0] && c5 == xi[1] && c6 == xi[2]) {
        if(c1==fu[0] && c2==fu[1] && c3==fu[2]) {
            //printf("reach 5");
            return "ジ";
        }
    }
    else if (c4==xe[0] && c5 == xe[1] && c6 == xe[2]) {
        if(c1==fu[0] && c2==fu[1] && c3==fu[2]) {
            //printf("reach 6");
            return "ゼ";
        }
    }
    else if (c4==xo[0] && c5 == xo[1] && c6 == xo[2]) {
        if(c1==fu[0] && c2==fu[1] && c3==fu[2]) {
            //printf("reach 7");
            return "ゾ";
        }
    }
    else {
        //printf("reach else\n");
        //printf("%s\n", keitaisos[ii].hinshi);
        //助詞の例外処理をここで行う
        if(strcmp(keitaisos[ii].hinshi, "助詞")==0) {
            //printf("reach joshi\n");
            if(c4==ga[0] && c5 == ga[1] && c6 == ga[2]) {
                return "グ";
            }
            if(c4==no[0] && c5 == no[1] && c6 == no[2]) {
                return "ン";
            }
            if(c4==ha[0] && c5 == ha[1] && c6 == ha[2]) {
                return "パ";
            }
            if(c4==wo[0] && c5 == wo[1] && c6 == wo[2]) {
                return "ゾ";
            }
            if(c4==de[0] && c5 == de[1] && c6 == de[2]) {
                return "グ";
            }
            if(c4==to[0] && c5 == to[1] && c6 == to[2]) {
                return "ド";
            }
            else {
                return japgro[j].gro;
            }
        }
        else {
            return japgro[j].gro;
        }
    }
}



void translate(int num_word, keitaiso *keitaisos) {
    char *word = malloc(sizeof(char)*100);
    int word_len = 0;
    int word_count = 0;
    int flag = 0;
    //printf("%d\n", num_word);
    char result_sentence[1000] = "";
        
    for(int i=0; i<num_word; i++) {
        word_len = 0;
        word_count=0;
        char result_word[300] = "";
        strcpy(word, keitaisos[i].word);
        word_len = strlen(word);
        //printf("%s %d\n", word, word_len);
        while(word_count<word_len) {
            flag = 0;
            if(strcmp(keitaisos[i].hinshi2, "固有名詞")==0 || strcmp(keitaisos[i].word, "クウガ")==0) {
                strcat(result_word, keitaisos[i].word);
                break;
            }
            for(int j=0; j<N; j++) {
                if(japgro[j].jap[0]==word[word_count] && japgro[j].jap[1]==word[word_count+1] && japgro[j].jap[2]==word[word_count+2]) {
                    if(word_count==0) {
                        if(strcmp(keitaisos[i].hinshi, "助詞")==0) {
                            if(word[word_count]==ga[0] && word[word_count+1] == ga[1] && word[word_count+2] == ga[2]) {
                                strcat(result_word, "グ");
                            }
                            if(word[word_count]==no[0] && word[word_count+1] == no[1] && word[word_count+2] == no[2]) {
                                strcat(result_word, "ン");
                            }
                            if(word[word_count]==ha[0] && word[word_count+1] == ha[1] && word[word_count+2] == ha[2]) {
                                strcat(result_word, "パ");
                            }
                            if(word[word_count]==wo[0] && word[word_count+1] == wo[1] && word[word_count+2] == wo[2]) {
                                strcat(result_word, "ゾ");
                            }
                            if(word[word_count]==de[0] && word[word_count+1] == de[1] && word[word_count+2] == de[2]) {
                                strcat(result_word, "グ");
                            }
                            if(word[word_count]==to[0] && word[word_count+1] == to[1] && word[word_count+2] == to[2]) {
                                strcat(result_word, "ド");
                            }
                            else {
                                strcat(result_word, japgro[j].gro);
                            }
                        }
                        else {
                            strcat(result_word, japgro[j].gro);
                        }
                    }
                    else {
                        strcat(result_word, excep_hand(word[word_count-3], word[word_count-2], word[word_count-1], word[word_count], word[word_count+1], word[word_count+2], i, j, word_count, word, result_word));
                    }
                    
                    flag = 1;
                    break;
                }
            }
            if(flag==0) {
                //printf("N");
            }
            word_count += 3;
        }
        strcat(result_sentence, result_word);
        strcat(result_sentence, " ");
        //printf("word_count = %d\n", word_count);
    }
    printf("%s\n", result_sentence);
}

int main(int argc, char **argv) {
    char *filename = "sentence.txt";

    grongi_init();
    
    read_sentence(argv[1]);

    int num_word = write_sentence(filename);

    translate(num_word, keitaisos);

    return 0;
}