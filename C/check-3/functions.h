/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * version: 0.4 */

#include <malloc.h>
#include <stdio.h>

#include "structs.h"

#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/* Read one line from FILE pointer fp and store it in string line which is passed by reference.
 * If memory cannot be allocated, print error message and set line pointer to NULL.
 * If letter is not A, C, G, or T, print error message and set line pointer to NULL. */
void getLine(FILE *fp, char **line);

/* Allocate passed S structure and initialize its number of triplets to 0.
 * If memory cannot be allocated return 1, otherwise return 0. */
int initializeS(int t_max, int t_length, S_t *Sij);

/* Allocate passed L structure and initialize its number of triplets to 0 and row to 0.
 * If memory cannot be allocated return 1, otherwise return 0. */
int initializeL(int max_L, int t_max, int t_length, L_t **arrayL);

/* Based on a pattern, allocate and calculate maxLength matrix
 * If memory cannot be allocated return 1, otherwise return 0. */
int getMaxLength(char *pattern, int m, int ***maxLength);

/* Calculate c and f, based on Sij triplets and position of rightmost checked character j. */
void getCFfromS(S_t Sij, int j, int *c, int *f);

/* Copy triplets from L to newL (and set numTriplets accordingly). */
void copyTripletsToL(L_t oldL, int t_lenght, L_t *newL);

/* Copy triplets from L to S (and sets numTriplets accordingly). */
void copyTripletsToS(L_t oldL, int t_lenght, S_t *newS);

/* Add one triplet to L (and set numTriplets accordingly).
 * Variable p+1 is first index of sub-text, c+1 is first index of sub-pattern,
 * and f is number of matches between them. */
void addTripletToL(int p, int c, int f, L_t *newL);

/* Calculate maximum of three values and store it in max.
 * Return which choice (1, 2 or 3) is used. */
int getMax(int a, int b, int c, int *max);

/* Switch L with another L. */
void switchL(L_t **L1, L_t **L2);

/* Set all rows to -1, and all numTriples to 0 in L structure. */
void clearL(int lengthL, L_t *L);

/* Deallocate memory of S structure. */
void cleanupS(int t_max, S_t *Sij);

/* Deallocate memory of L structure. */
void cleanupL(int max_L, int t_max, L_t *L);

/* Deallocate memory of maxLength matrix. */
void cleanupMaxLength(int m, int **maxLength);

#endif // FUNCTIONS_H_INCLUDED
