#include "bwt.h"
#include <malloc.h>
#include <string.h>

#ifndef NULL
#define NULL 0
#endif

#define NO_OF_CHAR 256

int char_count[NO_OF_CHAR];
int M[NO_OF_CHAR];

void updateCharCount(char * chs, int len)
{
	int i;
	for (i = 0; i < NO_OF_CHAR; i++) {
		char_count[i] = 0;
	}
	for (i = 0; i < len; i++) {
		char_count[chs[i]]++;
	}
}

/**
 * quick sort
 */
void quickSort(char * buf, int* list, int low, int high)
{
    if(low >= high)
        return;
    int first = low;
    int last = high;
    int key = list[first];
    while (first != last)
    {
		while(first < last && strcmp(buf+list[last], buf+key) > 0) last--;
        list[first] = list[last];
        while(first < last && strcmp(buf+list[first],buf+key) < 0) first++;
        list[last] = list[first];
    }
    list[first] = key;
    quickSort(buf, list, low, first-1);
    quickSort(buf, list, first+1, high);
}

/**
 * Burrows-Wheeler transform
 */
int transform(char * chs, int len)
{
	/* new copy */
	char * T = (char*) malloc(sizeof(char) * (len + 1));
	memcpy(T, chs, sizeof(char) * len);
	T[len] = 0;

	/* count char */
	updateCharCount(chs, len);

	char * L = chs;
	int L_index = 0;

	int eof_index = 0;
	int i, j;
	for (i = 0; i < NO_OF_CHAR; i++) {
		int c_count = char_count[i];
		if (c_count == 0)
			continue;

		int * sort_index = (int*) malloc(sizeof(int) * c_count);
		int sort_index_index = 0;
		for (j = 0; j < len; j++) {
			if (T[j] == i)
				sort_index[sort_index_index++] = j;
		}

		quickSort(T, sort_index, 0, c_count-1);

		for (j = 0; j < c_count; j++) {
			int index = sort_index[j];
			if (index == 0) {
				L[L_index + j] = T[len - 1];
				eof_index = L_index + j;
			} else {
				L[L_index + j] = T[index - 1];
			}
		}
		L_index += c_count;
		free(sort_index);
	}
	free(T);
	return eof_index;
}

void updateM(char * chs, int len)
{
	int i;
	for (i = 0; i < NO_OF_CHAR; i++) {
		M[i] = 0;
	}
	updateCharCount(chs, len);

	for (i = 1; i < NO_OF_CHAR; i++) {
		M[i] = M[i-1] + char_count[i-1];
	}
}

void updateCharCum(char * chs, int * char_cum, int len, int eof_index)
{
	memset(char_cum, 0, sizeof(int) * len);
	int i, j;
	char c;
	for (i = 0; i < len; i++) {
		c = chs[i];
		for (j = i - 1; j >= 0; j--) {
			if (c == chs[j]) {
				char_cum[i] = char_cum[j] + 1;
				break;
			}
		}
	}

	char_cum[eof_index] = 0;
	c = chs[eof_index];
	for (i = 0; i < eof_index; i++) {
		if (c == chs[i])
			char_cum[i]++;
	}
}

/**
 * Burrows-Wheeler reverse
 */
void reverse(char * chs, int len, int eof_index)
{
	updateM(chs, len);

	int * char_cum = (int*) malloc(sizeof(int) * len);
	updateCharCum(chs, char_cum, len, eof_index);

	char * LF = chs;
	char * T = (char*) malloc(sizeof(char) * len);
	memcpy(T, chs, sizeof(char) * len);

	int pos = eof_index, i;
	char c;
	for (i = len - 1; i >= 0; i--) {
		c = T[pos];
		LF[i] = c;
		pos = char_cum[pos] + M[c];
	}
	free(T);
	free(char_cum);
}
