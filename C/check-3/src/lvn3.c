/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * author: Alen Skvaric
 * version: 0.8.1 */

#ifdef __linux__
#define LINUX
#endif // __linux__

#ifdef _WIN32
#define WINDOWS
#endif // _WIN32

#ifdef TEST
#include <time.h>
#include <unistd.h>
#ifdef WINDOWS
#include "windows.h"
#include "psapi.h"
#endif // WINDOWS
#endif // TEST

#include <ctype.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "macros.h"
#include "structs.h"

int main(int argc, char *argv[]) {

    FILE *fp;

    #ifdef TEST
    char *outputPerformanceFile = "performanceOutput";
    #endif // TEST

    #ifndef TEST
    char *outputResultFile = "resultOutput";
    #endif // TEST

    char *pattern;
    char *text;
    int m; // pattern length
    int n; // text length
    int k; // maximum number of differences allowed

    S_t Sij;

    /* Only 2 L_t structures are used in this implementation because for every iteration of diagonal d,
     * it is sufficient to have L values for current and previous e (number of difference). */
    L_t *prevL; // previous L (e of L equals previous e)
    L_t *currL; // current L (e of L equals current e)

    /* maxLength is m x m symmetric matrix, formed from pattern.
     * maxLength[i][j] is the longest match of prefixes between these
     * two suffixes: pattern[i,m] and pattern[j,m]. */
    int **maxLength;

    int i; // current starting text index (incremented by 1 after every iteration)
    int j = 0; // index of the rightmost text symbol that was checked in previous iterations of i
    int maxJ = 0; // index of the rightmost text symbol for current iteration i
    int maxD; // diagonal on which maxJ was first reached
    int e; // current number of difference between pattern and part of text (e=[0,k])
    int d; // current diagonal (|d|<=e)
    int isDone = 0; // boolean value, check if current iteration i has already better match (is already done)
    int noNewSymbol; // boolean value, check if no new rightmost text symbol is reached
    int l1; // row of L(d,e) after initialization
    int row; // row of L(d,e)
    int choice; // check from which diagonal did l1 (initial row value) come

    /* Triple T(p, c, f). p = i+row+d */
    int c; // c+1 - first index of sub-pattern
    int f; // number of matches between sub-text and sub-pattern where first index of
           // matched sub-text must be p+1, and first index of matched sub-pattern must be c+1

    int g; // g = maxLength[c][row]

    int x; // auxiliary variable
	
	#if !defined(LINUX) && !defined(WINDOWS)
	fprintf(stderr, "OS is not Linux or WIndows.\n");
	exit(1);
	#endif // LINUX, WINDOWS
	
    if (argc != 4) {
        printf("Usage: lvn3 pattern_file text_file max_differences\n");
        exit(1);
    }

    #ifdef TEST
    clock_t beginClock, endClock;
    double timeElapsed;

    beginClock = clock(); // start clock
    #endif // TEST

    /* Parse pattern file. */
    fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Pattern file \"%s\" cannot be opened.\n", argv[1]);
        exit(1);
    }
    if (getFasta(fp, &pattern) == -1) {
        printf("error");
        exit(1);
    }
    if (fclose(fp) == EOF) {
        fprintf(stderr, "Error closing pattern file \"%s\".\n", argv[1]);
        exit(1);
    }

    /* Parse text file. */
    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        fprintf(stderr, "Text file \"%s\" cannot be opened.\n", argv[2]);
        exit(1);
    }
    if (getFasta(fp, &text) == -1) {
        exit(1);
    }
    if (fclose(fp) == EOF) {
        fprintf(stderr, "Error closing text file \"%s\".\n", argv[2]);
        exit(1);
    }

    /* Get pattern and text length. */
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

    /* Get k. */
    k = 0;
    for (x=0; x<strlen(argv[3]); x++) {
        if (!isdigit(argv[3][x])) {
            k = -1;
            break;
        }
    }
    if (k == -1) {
        fprintf(stderr, "Maximal number of differences must be positive number.\n");
        exit(1);
    }
    k = atoi(argv[3]);
    if (k >= m) {
        fprintf(stderr, "Maximal number of differences must be less than pattern length.\n");
        exit(1);
    }

    /* Initialize Sij, prevL and currL. */
    if (initializeS(MAX_TRIPLET(k), TRIPLET_LENGTH, &Sij) == 1) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (initializeL(MAX_L(k), MAX_TRIPLET(k), TRIPLET_LENGTH, &prevL) == 1) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }
    if (initializeL(MAX_L(k), MAX_TRIPLET(k), TRIPLET_LENGTH, &currL) == 1) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }

    /* Get maxLength. */
    if (getMaxLength(pattern, m, &maxLength) == 1) {
        fprintf(stderr, "Memory could not be allocated.\n");
        exit(1);
    }

    #ifndef TEST
    fp = fopen(outputResultFile, "w"); // start writing to outputResultFile
    fprintf(fp, "pattern file: %s\ntext file: %s\nk: %d\n\n", argv[1], argv[2], k);
	fprintf(fp, "%12s%10s%22s\n", "startIndex", "endIndex", "numberOfDifferences");
    #endif // TEST

    /* Landau-Vishkin-Nussinov algorithm. */
    for (i=0; i<=n-m+k; i++) { // for i>n-m+k, match is impossible
		isDone = 0; // 1 - print result for current i, 0 - do not print
        for (e=0; e<=k; e++) { // limits: e=[0,k]
			
            /* Initialize previous L (where necessary). */
            prevL[D(e+1, k)].row = prevL[D(-e-1, k)].row = NEG_INFINITY;
            prevL[D(-e, k)].row = e - 1;
            prevL[D(e, k)].row = -1;

            for (d=-e; d<=e; d++) { // limits: |d|<=e
                noNewSymbol = 0; // 0 - no new rightmost symbol reached, 1 - new symbol reached
                choice = getMax(prevL[D(d-1, k)].row, prevL[D(d, k)].row+1, prevL[D(d+1, k)].row+1, &l1);
                if (l1 > m) { // limits: l1<=m
                    l1 = m;
                }
                row = l1;
                if (choice == 1) { // L(d-1,e-1)
                    copyTripletsToL(prevL[D(d-1, k)], TRIPLET_LENGTH, &currL[D(d, k)]);
                    addTripletToL(J(i, l1, d)-1, 0, 0, &currL[D(d, k)]);
                } else if (choice == 2) { // L(d,e-1)+1
                    copyTripletsToL(prevL[D(d, k)], TRIPLET_LENGTH, &currL[D(d, k)]);
                    addTripletToL(J(i, l1, d)-1, 0, 0, &currL[D(d, k)]);
                } else { // L(d+1,e-1)+1
                    copyTripletsToL(prevL[D(d+1, k)], TRIPLET_LENGTH, &currL[D(d, k)]);
                }

                /* To have a match, row must be equal to m. */
                while((row<m) && (J(i, row, d)+1<=j)) { // limits: j=rightmost known text character
                    getCFfromS(Sij, J(i, row, d), &c, &f); // p=J(i, row, d)
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

                if (noNewSymbol == 0) { // new symbol reached
                    while((row<m) && (J(i, row, d)<n) && (pattern[row]==text[J(i, row, d)])) {
                        row++;
                    }
                }
                currL[D(d, k)].row = row;
                if (currL[D(d, k)].row > l1) {
                    addTripletToL(J(i, l1, d), l1, currL[D(d, k)].row-l1, &currL[D(d, k)]);
                }
                if ((currL[D(d, k)].row==m && isDone==0)) { // match found
                    #ifndef TEST
                    fprintf(fp, "%12d%10d%22d\n", i, J(i, m, d)-1, e);
                    #endif // TEST
					isDone = 1;
                }
                if (J(i, currL[D(d, k)].row, d) > maxJ) { // store diagonal that reached the biggest j
                    maxJ = J(i, currL[D(d, k)].row, d);
                    maxD = D(d, k);
                }
            }
            if (e != k) { // switch current and previous L until e reaches k, if e=k switch is pointless
                switchL(&prevL, &currL);
            }
        }
        if (maxJ > j) { // new rightmost symbol reached
            j = maxJ;
            copyTripletsToS(currL[maxD], TRIPLET_LENGTH, &Sij);
        }

        /* Initialize two L structure for next iteration. */
        clearL(MAX_L(k), prevL);
        clearL(MAX_L(k), currL);
    }

    #ifndef TEST
    printf("\nDONE\n");
    fprintf(fp, "\nDONE\n");

    if (fclose(fp) == EOF) { // finish writing to outputResultFile
        fprintf(stderr, "Error closing results output file \"%s\".\n", outputResultFile);
        exit(1);
    }
    #endif // TEST

    #ifdef TEST

    /* Get elapsed time. */
    endClock = clock();
    timeElapsed = (double)(endClock - beginClock) / CLOCKS_PER_SEC;
    printf("\nElapsed time: %f s\n", timeElapsed);

    /* Get memory consumption. */
	
    #ifdef WINDOWS
	PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    double memoryUsed = (double)pmc.PeakWorkingSetSize/(1024*1024);
    printf("\nMemory used: %f MB\n", memoryUsed);
	#endif // WINDOWS
	
	#ifdef LINUX
    double memoryUsed = getMemoryValue("VmHWM");
    printf("\nMemory used: %f MB\n", memoryUsed);
	#endif // LINUX
	
    /* Write performance results to outputPerformanceFile. */
    if (access(outputPerformanceFile, F_OK) == -1) { // outputPerformanceFile does not exist
        fp = fopen(outputPerformanceFile, "w"); // start writing to outputPerformanceFile
        fprintf(fp, "%-15s%-15s%-10s%-20s%-15s", "textLength", "patternLength", "k-value", "elapsedTime(sec)", "memUsage(MB)");
    } else { // outputPerformanceFile exists
        fp = fopen(outputPerformanceFile, "a"); // start writing to outputPerformanceFile
    }
    fprintf(fp, "\n%-15d%-15d%-10d%-20.3f%-15.3f", n, m, k, timeElapsed, memoryUsed);
    if (fclose(fp) == EOF) { // finish writing to outputPerformanceFile
        fprintf(stderr, "Error closing performance output file\"%s\".\n", outputPerformanceFile);
        exit(1);
    }
    #endif // TEST

    /* Free allocated memory. */
    free(pattern);
    free(text);
    cleanupS(MAX_TRIPLET(k), &Sij);
    cleanupL(MAX_L(k), MAX_TRIPLET(k), prevL);
    cleanupL(MAX_L(k), MAX_TRIPLET(k), currL);
    cleanupMaxLength(m, maxLength);

    return 0;
}
