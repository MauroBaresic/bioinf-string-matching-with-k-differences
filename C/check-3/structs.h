/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * version: 0.4 */

#ifndef STRUCTS_H_INCLUDED
#define STRUCTS_H_INCLUDED

typedef struct {
    int numTriplets;
    int **T; // array of triplets T(p, c, f)
} S_t;

typedef struct {
    int row;
    int numTriplets;
    int **T; // array of triplets T(p, c, f)
} L_t;

#endif // STRUCTS_H_INCLUDED
