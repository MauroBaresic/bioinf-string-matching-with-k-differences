#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *getLine(FILE *stream) {
    char *lineOld = NULL, *lineUpdated = NULL;
    size_t tmpSize = 0;
    int index = 0;
    char tmpCharacter;

    while (1) {
        tmpCharacter = getc(stream);

        tmpSize += sizeof(tmpCharacter);
        lineUpdated = realloc(lineOld, tmpSize);
        if (lineUpdated == NULL) {
            free(lineOld);
            lineOld = lineUpdated;
            break;
        } else {
            lineOld = lineUpdated;
        }

        if (tmpCharacter == EOF || tmpCharacter == '\n') {
            lineOld[index] = '\0';
            break;
        } else {
            lineOld[index++] = tmpCharacter;
        }
    }

    return lineOld;
}

int **getMaxLength(char *pattern, int patternLength) {
    int tmpRow, tmpDiagonal;
    int **maxLength;

    maxLength = (int**) malloc(patternLength * sizeof(int*));
    for (tmpRow=0; tmpRow<patternLength; tmpRow++) {
        maxLength[tmpRow] = malloc(patternLength * sizeof(int));
    }

    for (tmpDiagonal=0; tmpDiagonal<=patternLength-1; tmpDiagonal++) {
        if (pattern[patternLength-tmpDiagonal-1] == pattern[patternLength-1]) {
            maxLength[patternLength-1-tmpDiagonal][patternLength-1] = 1;
            maxLength[patternLength-1][patternLength-1-tmpDiagonal] = 1;
        } else {
            maxLength[patternLength-1-tmpDiagonal][patternLength-1] = 0;
            maxLength[patternLength-1][patternLength-1-tmpDiagonal] = 0;
        }
        for (tmpRow=patternLength-2-tmpDiagonal; tmpRow>=0; tmpRow--) {
            if (pattern[tmpRow] == pattern[tmpRow+tmpDiagonal]) {
                maxLength[tmpRow][tmpRow+tmpDiagonal] = 1 + maxLength[tmpRow+1][tmpRow+tmpDiagonal+1];
                maxLength[tmpRow+tmpDiagonal][tmpRow] = 1 + maxLength[tmpRow+1][tmpRow+tmpDiagonal+1];
            } else {
                maxLength[tmpRow][tmpRow+tmpDiagonal] = 0;
                maxLength[tmpRow+tmpDiagonal][tmpRow] = 0;
            }
        }
    }

    return maxLength;
}

int min(int a, int b, int c) {
    int m = a;
    if (m > b) m = b;
    if (m > c) m = c;
    return m;
}

int main(int argc, char *argv[]) {

    FILE *stream;
    char *pattern;
    char *text;
    int patternLength = 0;
    int textLength = 0;

    int i, j, l, d, e, row;
    int k = 3;

    stream = fopen("examples/example1.txt", "r");
    if (stream == NULL) {
        fprintf(stderr, "File cannot be opened.\n");
        exit(1);
    }
    pattern = getLine(stream);
    text = getLine(stream);
    if (pattern == NULL || text == NULL) {
        if (pattern != NULL) {
            free(pattern);
        }
        if (text != NULL) {
            free(pattern);
        }
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    patternLength = strlen(pattern);
    textLength = strlen(text);
    int **maxLength = getMaxLength(pattern, patternLength);

    int **matL = (int**) malloc(((k+1)*2+1) * sizeof(int*));
    for (i=0; i<(k+1)*2+1; i++) {
        matL[i] = malloc((k+3) * sizeof(int));
    }
    for (i=0; i<(k+1)*2+1; i++) {
        for (j=0; j<k+3; j++)
            matL[i][j] = -9;
    }
    for (i=0; i<=textLength-patternLength+k; i++) {
        for (d=-(k+1); d<=(k+1); d++) {
            matL[d+k+1][abs(d)] = -5;
            if (d < 0) {
                matL[d+k+1][abs(d)+1] = abs(d) - 1;
            } else {
                matL[d+k+1][abs(d)+1] = -1;
            }
        }
        for (e=0; e<=k; e++) {
            for (d=-e; d<=e; d++) {
                row = matL[d+k+1][e+1]+1;
                if (row < matL[d+k][e+1]) row = matL[d+k][e+1];
                if (row < matL[d+k+2][e+1]+1) row = matL[d+k+2][e+1]+1;
                if (row > patternLength) {
                    row = patternLength;
                }
                while(row<patternLength && i+row+d<textLength && pattern[row]==text[i+row+d]) {
                    row += 1;
                }
                matL[d+k+1][e+2] = row;
                if (matL[d+k+1][e+2] == patternLength && e == k) {
                    printf("START: %d, STRING: %.*s\n", i+1, patternLength+d, text+i);
                }
            }
        }
    }
    for (i=0; i<(k+1)*2+1; i++) {
        for (j=0; j<k+3; j++)
            printf("%d ", matL[i][j]);
        printf("\n");
    }

    for(i=0; i<patternLength; i++) {
        free(maxLength[i]);
    }
    free(maxLength);
    for(i=0; i<(k+1)*2+1; i++) {
        free(matL[i]);
    }
    free(matL);
    free(pattern);
    free(text);
    fclose(stream);
    return 0;
}
