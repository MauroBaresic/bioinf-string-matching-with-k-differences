/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * version: 0.5 */

/*#include <time.h>
#include "windows.h"
#include "psapi.h"*/

#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "macros.h"
#include "structs.h"

int main(int argc, char *argv[]) {

    /*clock_t begin, end;
    double time_spent;*/

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
    int isDone; // check if current diagonal has already better match
    int noNewSymbol; // check if no new rightmost text symbol is reached
    int l1; // row of L(d,e) after initialization
    int row; // row of L(d,e)
    int choice; // check from which diagonal l1 came
    int c; // first index of sub-pattern
    int f; // number of matches between sub-text and sub-pattern
    int g; // MAXLENGTH(c, row)

    int x; // auxiliary variable

    //begin = clock();

    if (argc != 3) {
        printf("Usage: lvn3 filename max_differences\n");
        exit(1);
    }
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "File cannot be opened.\n");
        exit(1);
    }
    getLine(fp, &pattern);
    if (pattern == NULL) {
        exit(1);
    }
    getLine(fp, &text);
    if (text == NULL) {
        exit(1);
    }
    if (fclose(fp) == EOF) {
        fprintf(stderr, "Error closing file.\n");
        exit(1);
    }
    m = strlen(pattern);
    if (m == 0) {
        fprintf(stderr, "Pattern has 0 characters.\n");
        exit(1);
    }
    n = strlen(text);
    if (n == 0) {
        fprintf(stderr, "Text has 0 characters.\n");
        exit(1);
    }
    if (n < m) {
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
        fprintf(stderr, "Maximal number of differences must be positive number.\n");
        exit(1);
    }
    k = atoi(argv[2]);
    if (k >= m) {
        fprintf(stderr, "Maximal number of differences must be less than pattern length.\n");
        exit(1);
    }
    if (initializeS(MAX_TRIPLET(k), TRIPLET_LENGTH, &Sij) == 1) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (initializeL(MAX_L(k), MAX_TRIPLET(k), TRIPLET_LENGTH, &L) == 1) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (initializeL(MAX_L(k), MAX_TRIPLET(k), TRIPLET_LENGTH, &tmpL) == 1) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (getMaxLength(pattern, m, &maxLength) == 1) {
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
    cleanupS(MAX_TRIPLET(k), &Sij);
    cleanupL(MAX_L(k), MAX_TRIPLET(k), L);
    cleanupL(MAX_L(k), MAX_TRIPLET(k), tmpL);
    cleanupMaxLength(m, maxLength);

    printf("\nDONE\n");

    /*end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime spent: %f sec\n", time_spent);

    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    printf("\nPhysical memory used: %d B\n", physMemUsedByMe);*/

    return 0;
}
