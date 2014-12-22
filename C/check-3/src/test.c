/* String matching algorithm with k differences for nucleotide sequences.
 * Algorithm authors: Gad M. Landau, Uzi Vishkin and Ruth Nussinov.
 *
 * author: Alen Skvaric
 * version: 0.8 */

#include "test.h"

int parseLine(char* line) {

	while (*line < '0' || *line > '9') {
		line++;
	}
	
	return atoi(line);
}

int getMemoryValue(char *memType) {

	FILE* file = fopen("/proc/self/status", "r");
	int result = -1;
	char line[100];

	while (fgets(line, 100, file) != NULL) {
		if (strncmp(line, memType, strlen(memType)) == 0) {
			result = parseLine(line)*1024;
			break;
		}
	}
	fclose(file);
	
	return result;
}