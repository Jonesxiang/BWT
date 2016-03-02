#include "bwt.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

void random_chs(char * chs, int len)
{
	int i;
	for (i = 0; i < len; i++) {
		chs[i] = (char) (rand() % 256);
	}
}

void print_chs(char * chs, int len)
{
	printf("\n------------\n");
	int i;
	for (i = 0; i < len; i++) {
		printf("%x ", chs[i] & 0xff);
	}
	printf("\n------------\n");
}

int equal_chs(char * chs1, int chs1_len, char * chs2, int chs2_len)
{
	if (chs1_len != chs2_len)
		return 0;
	int i;
	for (i = 0; i < chs1_len; i++) {
		if (chs1[i] != chs2[i])
			return 0;
	}
	return 1;
}

int main(int argc, char * argv[])
{
	const int len = 100;
	srand(time(NULL));
	char chs[len];
	random_chs(chs, len);
	print_chs(chs, len);

	char tmp[len];
	memcpy(tmp, chs, sizeof(char) * len);

	int eof_index = bwt_transform(chs, len);
	print_chs(chs, len);
	printf("eof_index:%d\n", eof_index);

	bwt_reverse(chs, len, eof_index);
	printf("equal:%d\n", equal_chs(chs, len, tmp, len));

//	char str[] = {'B', 'u', 'r', 'r', 'o', 'w', 's', '-', \
		'W', 'h', 'e', 'e', 'l', 'e', 'r', ' ', \
		't', 'r', 'a', 'n', 's', 'f', 'o', 'r', 'm', 0};

	char str[] = "banana";
	printf("%s\n", str);

	eof_index = bwt_transform(str, strlen(str));
	printf("%s\n", str);
	printf("eof_index:%d\n", eof_index);
	printf("%d\n", strlen(str));
	bwt_reverse(str, strlen(str), eof_index);
	printf("%s\n", str);
	
	return 0;	
}
