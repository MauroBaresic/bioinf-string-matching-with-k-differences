//version: 0.2

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int i;
    int j;
    int numTriples;
    int **T;
} S;

typedef struct {
    int row;
    int numTriples;
    int **T;
} L;

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

S initializeS(int k) {
    int i;
    S Sij;

    Sij.i = 0;
    Sij.j = 0;
    Sij.numTriples = 0;
    Sij.T = (int**) malloc((2*k+2) * sizeof(int*));
    for (i=0; i<(2*k+2); i++) {
        Sij.T[i] = malloc(3 * sizeof(int));
    }

    return Sij;
}

L **initializeL(int k) {
    int i, j, z;
    L** matL;

    matL = (L**) malloc((2*(k+1)+1) * sizeof(L*));
    for (i=0; i<(2*(k+1)+1); i++) {
        matL[i] = malloc((k+3) * sizeof(L));
    }

    for (i=0; i<(2*(k+1)+1);i++) {
        for (j=0; j<(k+3);j++) {
            matL[i][j].row=0;
            matL[i][j].numTriples=0;
            matL[i][j].T = (int**) malloc((2*k+2) * sizeof(int*));
            for (z=0; z<2*k+2; z++) {
                matL[i][j].T[z] = malloc(3 * sizeof(int));
            }
        }
    }

    return matL;
}

int **getMaxLength(char *pattern, int m) {
    int i, d;
    int **maxLength;

    maxLength = (int**) malloc(m * sizeof(int*));
    for (i=0; i<m; i++) {
        maxLength[i] = malloc(m * sizeof(int));
    }

    for (d=0; d<=m-1; d++) {
        if (pattern[m-d-1] == pattern[m-1]) {
            maxLength[m-1-d][m-1] = 1;
            maxLength[m-1][m-1-d] = 1;
        } else {
            maxLength[m-1-d][m-1] = 0;
            maxLength[m-1][m-1-d] = 0;
        }
        for (i=m-2-d; i>=0; i--) {
            if (pattern[i] == pattern[i+d]) {
                maxLength[i][i+d] = 1 + maxLength[i+1][i+d+1];
                maxLength[i+d][i] = 1 + maxLength[i+1][i+d+1];
            } else {
                maxLength[i][i+d] = 0;
                maxLength[i+d][i] = 0;
            }
        }
    }

    return maxLength;
}

int main(int argc, char *argv[]) {

    FILE *stream;
    char *pattern;
    char *text;
    S Sij;
    L** matL;
    int **maxLength;

    int m;
    int n;

    int a, b, i, j, d, e, z, g, c, f, l1, row, choice, repair, found, skip, maxJ, maxE, maxD;
    int k = 3;

    stream = fopen("examples/example1.txt", "r");
    if (stream == NULL) {
        fprintf(stderr, "File cannot be opened.\n");
        exit(1);
    }

    pattern = getLine(stream);
    text = getLine(stream);
    if (pattern == NULL || text == NULL) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    m = strlen(pattern);
    n = strlen(text);

    Sij = initializeS(k);

    matL = initializeL(k);

    maxLength = getMaxLength(pattern, m);


    for (i=0; i<(k+1)*2+1; i++) {
        for (j=0; j<k+3; j++)
            matL[i][j].row = -9;
    }

    j = 0;
    for (i=0; i<=n-m+k; i++) {
        maxJ = j;
        maxD = -1;
        maxE = -1;
        for (d=-(k+1); d<=(k+1); d++) {
            matL[d+k+1][abs(d)].row = -5;
            if (d < 0) {
                matL[d+k+1][abs(d)+1].row = abs(d) - 1;
            } else {
                matL[d+k+1][abs(d)+1].row = -1;
            }
        }
        for (e=0; e<=k; e++) {
            for (d=-e; d<=e; d++) {
                skip = 0;
                choice = 1;
                row = matL[d+k][e+1].row;
                l1 = matL[d+k][e+1].row;
                if (row < matL[d+k+1][e+1].row+1) {
                    row = matL[d+k+1][e+1].row+1;
                    l1 = matL[d+k+1][e+1].row+1;
                    choice = 2;
                }
                if (row < matL[d+k+2][e+1].row+1) {
                    row = matL[d+k+2][e+1].row+1;
                    l1 = matL[d+k+2][e+1].row+1;
                    choice = 3;
                }
                if (row > m) {
                    row = m;
                    l1 = m;
                }
                if (choice == 1) {
                    matL[d+k+1][e+2].numTriples = matL[d+k][e+1].numTriples + 1;
                    for (a=0; a<matL[d+k][e+1].numTriples; a++) {
                        for(b=0; b<3; b++) {
                            matL[d+k+1][e+2].T[a][b] = matL[d+k][e+1].T[a][b];
                        }
                    }
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][0] = i+l1+d-1;
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][1] = 0;
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][2] = 0;
                } else if (choice == 2) {
                    matL[d+k+1][e+2].numTriples = matL[d+k+1][e+1].numTriples + 1;
                    for (a=0; a<matL[d+k+1][e+1].numTriples; a++) {
                        for(b=0; b<3; b++) {
                            matL[d+k+1][e+2].T[a][b] = matL[d+k+1][e+1].T[a][b];
                        }
                    }
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][0] = i+l1+d-1;
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][1] = 0;
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][2] = 0;
                } else {
                    matL[d+k+1][e+2].numTriples = matL[d+k+2][e+1].numTriples;
                    for (a=0; a<matL[d+k+2][e+1].numTriples; a++) {
                        for(b=0; b<3; b++) {
                            matL[d+k+1][e+2].T[a][b] = matL[d+k+2][e+1].T[a][b];
                        }
                    }
                }
                if (row != m) {
                    while(i+row+d+1 <= j) {
                        found = 0;
                        for (z=0; z<Sij.numTriples; z++) {
                            if (Sij.T[z][2] == 0) {
                                if ((i+row+d+1) == Sij.T[z][0]+1) {
                                    c = 0;
                                    f = 0;
                                    found = 1;
                                    break;
                                }
                            } else {
                                if ((i+row+d+1) <= (Sij.T[z][0]+Sij.T[z][2]) &&
                                (i+row+d+1) > Sij.T[z][0]) {
                                    repair = (i+row+d) - Sij.T[z][0];
                                    c = Sij.T[z][1] + repair;
                                    f = Sij.T[z][2] - repair;
                                    found = 1;
                                    break;
                                }
                            }
                        }
                        if (found == 1) {
                            if (f == 0) {
                                if (text[i+row+d] != pattern[row]) {
                                    skip = 1;
                                    break;
                                } else {
                                    row++;
                                }
                            } else {
                                if (f != maxLength[c][row]) {
                                    g = maxLength[c][row];
                                    if (g > f) {
                                        row += f;
                                    } else {
                                        row += g;
                                    }
                                    skip = 1;
                                    break;
                                } else {
                                    row += f;
                                }
                            }
                        }
                    }
                }
                if (skip == 0) {
                    while(row<m && i+row+d<n && pattern[row]==text[i+row+d]) {
                        row += 1;
                    }
                }
                matL[d+k+1][e+2].row = row;
                if (matL[d+k+1][e+2].row > l1) {
                    matL[d+k+1][e+2].numTriples++;
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][0] = i+l1+d;
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][1] = l1;
                    matL[d+k+1][e+2].T[matL[d+k+1][e+2].numTriples-1][2] = matL[d+k+1][e+2].row-l1;
                }
                if (matL[d+k+1][e+2].row == m && e == k) {
                    printf("START: %d, STRING: %.*s\n", i+1, m+d, text+i);
                }
                if (matL[d+k+1][e+2].row+d+i > maxJ) {
                    maxJ = matL[d+k+1][e+2].row+d+i;
                    maxD = d+k+1;
                    maxE = e+2;
                }
            }
        }
        Sij.i++;
        if (maxJ > j) {
            j = maxJ;
            Sij.j = maxJ;
            Sij.numTriples = matL[maxD][maxE].numTriples;
            for (a=0; a<matL[maxD][maxE].numTriples; a++) {
                for(b=0; b<3; b++) {
                    Sij.T[a][b] = matL[maxD][maxE].T[a][b];
                }
            }
        }
    }

    free(pattern);
    free(text);
    fclose(stream);
    return 0;
}
