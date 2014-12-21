/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * author: Alen Skvaric
 * version: 0.7.1 */

#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

/* S_t represents S(i,j) which consists of list of triplets T(p, c, f)
 *   i - starting text index at the moment Sij was created
 *   j - index of the rightmost reached text symbol at the moment Sij was created
 *   p+1 - first index of sub-text
 *   c+1 - first index of sub-pattern
 *   f - number of matches between sub-text and sub-pattern where first index of
         matched sub-text must be p+1, and first index of matched sub-pattern must be c+1 */
typedef struct {
    int numTriplets; // number of triplets T(p, c, f)
    int **T; // array of triplets T(p, c, f)
} S_t;

/* L_t represents L(d,e) which consists of list of triplets T(p, c, f) and row value
 * which represents largest row such that matD(i,j)=e and matD(row,j) is on diagonal d
 * matD - matrix of gap cost
 * d - diagonal (|d|<=e)
 * e - number of difference between pattern and sub-text (e=[0,k])
 * p+1 - first index of sub-text
 * c+1 - first index of sub-pattern
 * f - number of matches between sub-text and sub-pattern where first index of
       matched sub-text must be p+1, and first index of matched sub-pattern must be c+1 */
typedef struct {
    int row; // largest row such that matD(i,j)=e and matD(row,j) is on diagonal d
    int numTriplets; // number of triplets T(p, c, f)
    int **T; // array of triplets T(p, c, f)
} L_t;

#endif // STRUCTS_H_INCLUDED
