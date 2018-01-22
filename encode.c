#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    int i;
    char a[100] = "bash use_mecab.sh ";
    char *filename = "sentence.txt";
    char text[10][20];
    FILE *fp;
    strcat(a, argv[1]);

    system(a);

    if ((fp = fopen(filename, "r")) == NULL) {
        fprintf(stderr, "%s\n", "error: can't read file.");
        return EXIT_FAILURE;
    }

    while (fscanf(fp, "%s\t%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",&text[0],&text[1],&text[2],&text[3],text[4],&text[5],&text[6],&text[7],&text[8],&text[9]) != EOF){
        printf("%s %s %s %s %s %s %s %s %s %s \n", text[0],text[1],text[2],text[3],text[4],text[5],text[6],text[7],text[8],text[9]);
        //printf("%s\n", text[8]);
    }

    return 0;
}