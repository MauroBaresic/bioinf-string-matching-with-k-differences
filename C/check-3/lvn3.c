//version: 0.3

#include <assert.h>
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NEG_INFINITY -10
#define TRIPLET_LENGTH 3
#define MAX_TRIPLET(k) (2*k+2)
#define MAX_L(k) (2*(k+1)+1)
#define D(d, k) (d+k+1)
#define J(i, row, d) (i+row+d)

typedef struct {
    int numTriplets;
    int **T; // array of triplets T(p, c, f)
} S_t;

typedef struct {
    int row;
    int numTriplets;
    int **T; // array of triplets T(p, c, f)
} L_t;

// reads one line from pf
int getLine(FILE *fp, char **line) {

    char *lineOld = NULL;
    char *lineUpdated;

    char character;
    size_t lineSize = 0;
    int index = 0;
    int isEOF = 0;

    while (1) {
        character = getc(fp);
        lineSize += sizeof(character);
        lineUpdated = realloc(lineOld, lineSize);
        if (lineUpdated == NULL) {
            free(lineOld);
            lineOld = NULL;
            fprintf(stderr, "Memory could not be allocated.\n");
            break;
        } else {
            lineOld = lineUpdated;
        }

        if (character == EOF || character == '\n') {
            lineOld[index] = '\0';
            if (character == EOF) {
                isEOF = 1;
            }
            break;
        } else if (character != 'A' && character != 'C' && character != 'G' && character != 'T') {
            free(lineOld);
            lineOld = NULL;
            fprintf(stderr, "Only characters A, C, G and T (upper case) are allowed.\n");
            break;
        } else {
            lineOld[index] = character;
            index++;
        }
    }
    *line = lineOld;
    return isEOF;
}

int initializeS(int k, S_t *Sij) {

    S_t tmpSij;
    int error = 0; // allocation error

    int x, y;

    tmpSij.numTriplets = 0;
    tmpSij.T = (int**) malloc(MAX_TRIPLET(k) * sizeof(int*));
    if (tmpSij.T == NULL) {
        error = 1;
        return error;
    }
    for (x=0; x<MAX_TRIPLET(k); x++) {
        tmpSij.T[x] = malloc(TRIPLET_LENGTH * sizeof(int));
        if (tmpSij.T[x] == NULL) {
            error = 1;
            for (y=0; y<x; y++) {
                free(tmpSij.T[y]);
            }
            free(tmpSij.T);
            break;
        }
    }
    if (error == 0) {
        *Sij = tmpSij;
    }
    return error;
}

int initializeL(int k, L_t **arrayL) {

    L_t *tmpArrayL;
    int error = 0; // allocation error

    int x, y, z; //auxiliary variables

    tmpArrayL = (L_t*) malloc(MAX_L(k) * sizeof(L_t));
    if (tmpArrayL == NULL) {
        error = 1;
        return error;
    }

    for (x=0; x<MAX_L(k); x++) {
        tmpArrayL[x].row=-1;
        tmpArrayL[x].numTriplets=0;
        tmpArrayL[x].T = (int**) malloc(MAX_TRIPLET(k) * sizeof(int*));
        if (tmpArrayL[x].T == NULL) {
            error = 1;
            for (z=0; z<x; z++) {
                free(tmpArrayL[z].T);
            }
            free(tmpArrayL);
            break;
        }
        for (y=0; y<MAX_TRIPLET(k); y++) {
            tmpArrayL[x].T[y] = malloc(TRIPLET_LENGTH * sizeof(int));
            if (tmpArrayL[x].T[y] == NULL) {
                error = 1;
                for (z=0; z<y; z++) {
                    free(tmpArrayL[x].T[z]);
                }
                for (z=0; z<x; z++) {
                    free(tmpArrayL[z].T);
                }
                free(tmpArrayL);
                break;
            }
        }
        if (error == 1) {
            break;
        }
    }
    if (error == 0) {
        *arrayL = tmpArrayL;
    }
    return error;
}

int getMaxLength(char *pattern, int m, int ***maxLength) {

    int i; // row in maxLength matrix
    int d; // diagonal in maxLength matrix
    int error = 0; // allocation error
    int **tmpMaxLength;

    int x; // auxiliary variable

    tmpMaxLength = (int**) malloc(m * sizeof(int*));
    if (tmpMaxLength == NULL) {
        error = 1;
        return error;
    }
    for (i=0; i<m; i++) {
        tmpMaxLength[i] = malloc(m * sizeof(int));
        if (tmpMaxLength[i] == NULL) {
            error = 1;
            for (x=0; x<i; x++) {
                free(tmpMaxLength[x]);
            }
            free(tmpMaxLength);
            break;
        }
    }
    if (error == 0) {
        for (d=0; d<=m-1; d++) {
            if (pattern[m-d-1] == pattern[m-1]) {
                tmpMaxLength[m-1-d][m-1] = tmpMaxLength[m-1][m-1-d] = 1;
            } else {
                tmpMaxLength[m-1-d][m-1] = tmpMaxLength[m-1][m-1-d] = 0;
            }
            for (i=m-2-d; i>=0; i--) {
                if (pattern[i] == pattern[i+d]) {
                    tmpMaxLength[i][i+d] = tmpMaxLength[i+d][i] = tmpMaxLength[i+1][i+d+1] + 1;
                } else {
                    tmpMaxLength[i][i+d] = tmpMaxLength[i+d][i] = 0;
                }
            }
        }
        *maxLength = tmpMaxLength;
        return error;
    } else {
        return error;
    }
}

void getCFfromS(S_t Sij, int j, int *c, int *f) {
    int x; // auxiliary variable
    int repair; // calculates how much is j away from p (first index of sub-text)

    for (x=0; x<Sij.numTriplets; x++) {
        if (Sij.T[x][2] == 0) {
            if (j+1 == Sij.T[x][0]+1) {
                *c = *f = 0;
                break;
            }
        } else {
            if ((j+1<=Sij.T[x][0]+Sij.T[x][2]) && (j+1>Sij.T[x][0])) {
                repair = j - Sij.T[x][0];
                *c = Sij.T[x][1] + repair;
                *f = Sij.T[x][2] - repair;
                break;
            }
        }
    }
}

void copyTripletsToL(L_t oldL, int t_lenght, L_t *newL) {
    int x, y; // auxiliary variables

    for (x=0; x<oldL.numTriplets; x++) {
        for(y=0; y<t_lenght; y++) {
            newL->T[x][y] = oldL.T[x][y];
        }
    }
    newL->numTriplets = oldL.numTriplets;
}

void copyTripletsToS(L_t oldL, int t_lenght, S_t *newS) {
    int x, y; // auxiliary variables

    for (x=0; x<oldL.numTriplets; x++) {
        for(y=0; y<t_lenght; y++) {
            newS->T[x][y] = oldL.T[x][y];
        }
    }
    newS->numTriplets = oldL.numTriplets;
}

void addTripletToL(int p, int c, int f, L_t *newL) {
    newL->numTriplets++;
    newL->T[newL->numTriplets-1][0] = p;
    newL->T[newL->numTriplets-1][1] = c;
    newL->T[newL->numTriplets-1][2] = f;
}

int getMax(int a, int b, int c, int *min) {
    int choice = 1;
    int tmpMin;

    tmpMin = a;
    if (tmpMin < b) {
        tmpMin = b;
        choice = 2;
    }
    if (tmpMin < c) {
        tmpMin = c;
        choice = 3;
    }
    *min = tmpMin;
    return choice;
}

void switchL(L_t **L1, L_t **L2) {
    L_t *tmp;

    tmp = *L1;
    *L1 = *L2;
    *L2 = tmp;
}

void clearL(int lengthL, L_t *L) {
    int x; // auxiliary variable

    for (x=0; x<lengthL; x++) {
        L[x].row = -1;
        L[x].numTriplets = 0;
    }
}

int main(int argc, char *argv[]) {

    FILE *fp;

    char *pattern;
    char *text;
    int m; // pattern length
    int n; // text length
    int k; // maximal number of differences allowed

    S_t Sij;
    L_t *L;
    L_t *tmpL;
    int **maxLength;

    int i; // current starting text index
    int j = 0; // index of the rightmost text symbol that was checked in previous iterations
    int maxJ = 0; // current index of the rightmost text symbol
    int maxD; // diagonal on which maxJ was first reached
    int e; // current number difference between pattern and part of text
    int d; // current diagonal
    int isDone; // checks if current diagonal has already better match
    int noNewSymbol; // checks if no new rightmost text symbol is reached
    int l1; // row of L(d,e) after initialization
    int row; // row of L(d,e)
    int choice; // checks from which diagonal l1 came
    int c; // first index of sub-pattern
    int f; // number of matches between sub-text and sub-pattern
    int g; // MAXLENGTH(c, row)

    int x; // auxiliary variable

    if (argc != 3) {
        printf("Usage: lvn filename max_differences\n");
        exit(0);
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "File cannot be opened.\n");
        exit(1);
    }
    if (getLine(fp, &pattern) == 1 && pattern != NULL) {
        free(pattern);
        fprintf(stderr, "File has only one line.\n");
        exit(1);
    };
    if (pattern == NULL) {
        exit(1);
    }
    if (getLine(fp, &text) == 0 && text != NULL) {
        free(pattern);
        free(text);
        fprintf(stderr, "File has more than two lines.\n");
        exit(1);
    };
    if (text == NULL) {
        free(pattern);
        exit(1);
    }
    if (fclose(fp) == EOF) {
        free(pattern);
        free(text);
        fprintf(stderr, "Error closing file.\n");
        exit(1);
    }
    m = strlen(pattern);
    if (m == 0) {
        free(pattern);
        free(text);
        fprintf(stderr, "Pattern has 0 characters.\n");
        exit(1);
    }
    n = strlen(text);
    if (n == 0) {
        free(pattern);
        free(text);
        fprintf(stderr, "Text has 0 characters.\n");
        exit(1);
    }
    if (n < m) {
        free(pattern);
        free(text);
        fprintf(stderr, "Pattern has more characters then text.\n");
        exit(1);
    }
    k = 0;
    for (x=0; x<strlen(argv[2]); x++) {
        if (!isdigit(argv[2][x])) {
            k = -1;
            break;
        }
    }
    if (k == -1) {
        free(pattern);
        free(text);
        fprintf(stderr, "Maximal number of differences must be positive number.\n");
        exit(1);
    }
    k = atoi(argv[2]);
    if (k >= m) {
        free(pattern);
        free(text);
        fprintf(stderr, "Maximal number of differences must be less than pattern length.\n");
        exit(1);
    }
    if (initializeS(k, &Sij) == 1) {
        //free pattern, text
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (initializeL(k, &L) == 1) {
        //free pattern, text, Sij
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (initializeL(k, &tmpL) == 1) {
        //free pattern, text, Sij, L
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (getMaxLength(pattern, m, &maxLength) == 1) {
        //free pattern, text, Sij, L, tmpL
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    for (i=0; i<=n-m+k; i++) {
        for (e=0; e<=k; e++) {
            L[D(e+1, k)].row = L[D(-e-1, k)].row = NEG_INFINITY;
            L[D(-e, k)].row = e - 1;
            L[D(e, k)].row = -1;
            for (d=-e; d<=e; d++) {
                isDone = 0;
                noNewSymbol = 0;
                l1 = -1;
                if (L[D(d, k)].row == m) {
                    isDone = 1;
                }
                choice = getMax(L[D(d-1, k)].row, L[D(d, k)].row+1, L[D(d+1, k)].row+1, &l1);
                if (l1 > m) {
                    l1 = m;
                }
                row = l1;
                if (choice == 1) {
                    copyTripletsToL(L[D(d-1, k)], TRIPLET_LENGTH, &tmpL[D(d, k)]);
                    addTripletToL(J(i, l1, d)-1, 0, 0, &tmpL[D(d, k)]);
                } else if (choice == 2) {
                    copyTripletsToL(L[D(d, k)], TRIPLET_LENGTH, &tmpL[D(d, k)]);
                    addTripletToL(J(i, l1, d)-1, 0, 0, &tmpL[D(d, k)]);
                } else {
                    copyTripletsToL(L[D(d+1, k)], TRIPLET_LENGTH, &tmpL[D(d, k)]);
                }
                while((row<m) && (J(i, row, d)+1<=j)) {
                    getCFfromS(Sij, J(i, row, d), &c, &f);
                    if (f == 0) {
                        if (text[J(i, row, d)] != pattern[row]) {
                            noNewSymbol = 1;
                            break;
                        } else {
                            row++;
                        }
                    } else {
                        g = maxLength[c][row];
                        if (g >= f) {
                            row += f;
                        } else {
                            row += g;
                        }
                        if (f != g) {
                            noNewSymbol = 1;
                            break;
                        }
                    }
                }
                if (noNewSymbol == 0) {
                    while((row<m) && (J(i, row, d)<n) && (pattern[row]==text[J(i, row, d)])) {
                        row++;
                    }
                }
                tmpL[D(d, k)].row = row;
                if (tmpL[D(d, k)].row > l1) {
                    addTripletToL(J(i, l1, d), l1, tmpL[D(d, k)].row-l1, &tmpL[D(d, k)]);
                }
                if ((tmpL[D(d, k)].row==m) && (isDone==0)) {
                    printf("START: %d, END: %d, DIFF: %d -> %.*s\n", i+1, J(i, m, d), e, J(0, m, d), text+i);
                }
                if (J(i, tmpL[D(d, k)].row, d) > maxJ) {
                    maxJ = J(i, tmpL[D(d, k)].row, d);
                    maxD = D(d, k);
                }
            }
            if (e != k) {
                switchL(&L, &tmpL);
            }
        }
        if (maxJ > j) {
            j = maxJ;
            copyTripletsToS(tmpL[maxD], TRIPLET_LENGTH, &Sij);
        }
        clearL(MAX_L(k), L);
        clearL(MAX_L(k), tmpL);
    }

    free(pattern);
    free(text);
    //free Sij, L, tmpL, maxLength

    printf("DONE\n");
    return 0;
}
