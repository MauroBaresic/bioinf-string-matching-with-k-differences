/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * author: Alen Skvaric
 * version: 0.8 */

#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define NEG_INFINITY -10 // negative infinity for row value in algorithm
#define TRIPLET_LENGTH 3 // T(p, c, f)
#define MAX_TRIPLET(k) (2*k+2) // maximum number of triplets required in L or S structures
#define MAX_L(k) (2*(k+1)+1) // maximum number of L structures required
#define D(d, k) (d+k+1) // calculate row of diagonal d (for list of L_t structures)
#define J(i, row, d) (i+row+d) // calculate column (row+d) with offset i (for matrix of gap cost matD)

#endif // MACROS_H_INCLUDED
