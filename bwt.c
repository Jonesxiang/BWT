#include "bwt.h"
#include <malloc.h>
#include <memory.h>

#ifndef NULL
#define NULL 0
#endif

#define BWT_CHAR_NUM 256

void bwt_counting(char * chs, int chs_len, int * count, int count_len)
{
	memset(count, 0, sizeof(int) * count_len);
	int i;
	for (i = 0; i < chs_len; i++) {
		count[0xff & chs[i]]++;
	}
}


/**********************************************/
/* transform                                  */
/**********************************************/

int bwt_strcmp(char * str1, int str1_len, char * str2, int str2_len)
{
	int e = str1_len < str2_len ? str1_len : str2_len;
	int i;
	for (i = 0; i < e; i++) {
		int s1 = 0xff & str1[i];
		int s2 = 0xff & str2[i];
		if (s1 > s2)
			return 1;
		else if (s1 < s2)
			return -1;
	}
	if (str1_len > str2_len)
		return 1;
	else if (str1_len < str2_len)
		return -1;
	else 
		return 0; /* never happen */
	
}

/**
 * quick sort
 */
void bwt_quicksort(char * buf, int len, int * index_list, int low, int high)
{
	if(low >= high) 
		return;
	int first = low;
	int last = high;
	int key = index_list[first];
	while (first != last)
	{
		while(first < last && bwt_strcmp(buf+index_list[last],
				       len - index_list[last], buf+key, len - key) > 0) 
			last--;
		index_list[first] = index_list[last]; 
		while(first < last && bwt_strcmp(buf+index_list[first], 
					len - index_list[first], buf+key, len - key) < 0) 
			first++;
		index_list[last] = index_list[first];
	}
	index_list[first] = key;
	bwt_quicksort(buf, len, index_list, low, first-1);
	bwt_quicksort(buf, len, index_list, first+1, high);
}

/**
 * Burrows-Wheeler transform
 */
int bwt_transform(char * chs, int len)
{
	/* 字符数组拷贝 */
	char * T = (char*) malloc(sizeof(char) * (len));
	memcpy(T, chs, sizeof(char) * len);

	/* 计算每种字符的数量 */
	int char_count[BWT_CHAR_NUM];
	bwt_counting(chs, len, char_count, BWT_CHAR_NUM);

	/* 转换结果 */
	char * L = chs;
	int L_index = 0;

	/* T中的最后一个字符对应于L中的下标 */
	int eof_index = 0;

	int c;
	for (c = 0; c < BWT_CHAR_NUM; c++) {
		/* 获取字符i的个数 */
		int c_count = char_count[c];
		if (c_count == 0)
			continue;

		/* 保存各字符c在T中的下标 */
		int * c_indexs = (int*) malloc(sizeof(int) * c_count);
		int c_indexs_i = 0, i;
		for (i = 0; i < len; i++) {
			if ((0xff & T[i]) == c)
				c_indexs[c_indexs_i++] = i;
		}
		
		/* 对以字符c开头的字符串进行排序, 排序的结果以下标呈现 */
		bwt_quicksort(T, len, c_indexs, 0, c_count-1);

		for (i = 0; i < c_count; i++) {
			int index = c_indexs[i];
			if (index == 0) {
				L[L_index + i] = T[len - 1];
				eof_index = L_index + i;
			} else {
				L[L_index + i] = T[index - 1];
			}
		}
		L_index += c_count;
		free(c_indexs);
	}
	free(T);
	return eof_index;
}

/**********************************************/
/* reverse                                    */
/**********************************************/

void bwt_count_M(char * chs, int chs_len, int * M, int M_len)
{
	int char_count[M_len];
	bwt_counting(chs, chs_len, char_count, M_len);
	M[0] = 0;

	int i;
	for (i = 1; i < M_len; i++) {
		M[i] = M[i-1] + char_count[i-1];
	}
}

void bwt_count_cum(char * chs, int * cum, int len, int eof_index)
{
	memset(cum, 0, sizeof(int) * len);

	int i, j;
	char c;
	for (i = 0; i < len; i++) {
		c = chs[i];
		for (j = i - 1; j >= 0; j--) {
			if (c == chs[j]) {
				cum[i] = cum[j] + 1;
				break;
			}
		}
	}

	cum[eof_index] = 0;
	c = chs[eof_index];
	for (i = 0; i < eof_index; i++) {
		if (c == chs[i])
			cum[i]++;
	}
}

/**
 * Burrows-Wheeler reverse
 */
void bwt_reverse(char * chs, int len, int eof_index)
{
	int M[BWT_CHAR_NUM];
	bwt_count_M(chs, len, M, BWT_CHAR_NUM);

	int * char_cum = (int*) malloc(sizeof(int) * len);
	bwt_count_cum(chs, char_cum, len, eof_index);

	char * LF = chs;
	char * T = (char*) malloc(sizeof(char) * len);
	memcpy(T, chs, sizeof(char) * len);

	int pos = eof_index, i;
	char c;
	for (i = len - 1; i >= 0; i--) {
		c = T[pos];
		LF[i] = c;
		pos = char_cum[pos] + M[c & 0xff];
	}
	free(T);
	free(char_cum);
}
