/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * version: 0.4 */

#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define NEG_INFINITY -10 // represent negative infinity for row value in algorithm
#define TRIPLET_LENGTH 3 // T(p, c, f)
#define MAX_TRIPLET(k) (2*k+2) // maximal number of triplets required in L or S structures
#define MAX_L(k) (2*(k+1)+1) // maximal number of L structures required
#define D(d, k) (d+k+1) // calculate diagonal row in stored array
#define J(i, row, d) (i+row+d) // calculate column (row+d) with offset i

#endif // MACROS_H_INCLUDED
