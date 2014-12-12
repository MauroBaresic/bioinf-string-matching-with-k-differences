//version: 0.1

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

    int i, j, l;
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

    /*for (i=0; i<patternLength; i++) {
        for (j=0; j<patternLength; j++)
            printf("%d ", maxLength[i][j]);
        printf("\n");
    }*/

    int **matD = (int**) malloc((patternLength+1) * sizeof(int*));
    for (i=0; i<patternLength+1; i++) {
        matD[i] = malloc((patternLength+k+1) * sizeof(int));
    }
    for (i=0; i<patternLength+1; i++) {
        for (j=0; j<patternLength+k+1; j++)
            matD[i][j] = -1;
    }
    for (i=0; i<=textLength-patternLength+k; i++) { // <=textLength-patternLength+k
        matD[0][0] = 0;
        for (j=1; j<=patternLength+k; j++) {
            matD[0][j] = j;
        }
        for (l=1; l<=patternLength; l++) {
            matD[l][0] = l;
        }
        for (l=1; l<=patternLength; l++) {
            for (j=1; j<=patternLength+k; j++) {
                if (i+j <= textLength && pattern[l-1] == text[i+j-1]) {
                    matD[l][j] = min(matD[l-1][j]+1, matD[l][j-1]+1, matD[l-1][j-1]);
                } else {
                    matD[l][j] = min(matD[l-1][j]+1, matD[l][j-1]+1, matD[l-1][j-1]+1);
                }
            }
        }
        for (j=patternLength-k; j<=patternLength+k; j++) {
            if (matD[patternLength][j] <= k) {
                printf("START: %d, STRING: %.*s\n", i+1, j, text+i);
            }
        }
    }
    for (i=0; i<patternLength+1; i++) {
        for (j=0; j<patternLength+k+1; j++)
            printf("%d ", matD[i][j]);
        printf("\n");
    }

    for(i=0; i<patternLength; i++) {
        free(maxLength[i]);
    }
    free(maxLength);
    for(i=0; i<=patternLength; i++) {
        free(matD[i]);
    }
    free(matD);
    free(pattern);
    free(text);
    fclose(stream);
    return 0;
}
