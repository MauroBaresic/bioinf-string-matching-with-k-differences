/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * author: Alen Skvaric
 * version: 0.9 */

#ifndef TEST_H_INCLUDED
#define TEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST // enable performance testing

/* Find first occurence of a number and return it. */
int parseLine(char* line);

/* Open "/proc/self/status" and find memory value of specified memory type.
 * If no value is found, return negative value.
 * NOTE: Return value is in megabytes. */
double getMemoryValue(char* memType);

#endif // TEST_H_INCLUDED