/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * version: 0.4 */

#include "functions.h"

void getLine(FILE *fp, char **line) {

    char *lineOld = NULL;
    char *lineUpdated;

    char character;
    size_t lineSize = 0;
    int index = 0;

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
}

int initializeS(int t_max, int t_length, S_t *Sij) {

    S_t tmpSij;
    int error = 0; // allocation error

    int x, y;

    tmpSij.numTriplets = 0;
    tmpSij.T = (int**) malloc(t_max * sizeof(int*));
    if (tmpSij.T == NULL) {
        error = 1;
        return error;
    }
    for (x=0; x<t_max; x++) {
        tmpSij.T[x] = malloc(t_length * sizeof(int));
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

int initializeL(int max_L, int t_max, int t_length, L_t **arrayL) {

    L_t *tmpArrayL;
    int error = 0; // allocation error

    int x, y, z; //auxiliary variables

    tmpArrayL = (L_t*) malloc(max_L * sizeof(L_t));
    if (tmpArrayL == NULL) {
        error = 1;
        return error;
    }

    for (x=0; x<max_L; x++) {
        tmpArrayL[x].row=-1;
        tmpArrayL[x].numTriplets=0;
        tmpArrayL[x].T = (int**) malloc(t_max * sizeof(int*));
        if (tmpArrayL[x].T == NULL) {
            error = 1;
            for (z=0; z<x; z++) {
                free(tmpArrayL[z].T);
            }
            free(tmpArrayL);
            break;
        }
        for (y=0; y<t_max; y++) {
            tmpArrayL[x].T[y] = malloc(t_length * sizeof(int));
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
    int repair; // calculate how much is j away from p (first index of sub-text)

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

int getMax(int a, int b, int c, int *max) {
    int choice = 1;
    int tmpMax;

    tmpMax = a;
    if (tmpMax < b) {
        tmpMax = b;
        choice = 2;
    }
    if (tmpMax < c) {
        tmpMax = c;
        choice = 3;
    }
    *max = tmpMax;
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

void cleanupS(int t_max, S_t *Sij) {
    int x; // auxiliary variable

    for (x=0; x<t_max; x++) {
        free(Sij->T[x]);
    }
    free(Sij->T);
}

void cleanupL(int max_L, int t_max, L_t *L) {
    int x, y; // auxiliary variables

    for (x=0; x<max_L; x++) {
        for (y=0; y<t_max; y++) {
            free(L[x].T[y]);
        }
        free(L[x].T);
    }
    free(L);
}

void cleanupMaxLength(int m, int **maxLength) {
    int x; // auxiliary variable

    for (x=0; x<m; x++) {
        free(maxLength[x]);
    }
    free(maxLength);
}
