#include "bwt.h"
#include "file.h"
#include <unistd.h>

const char * __progname = "bwt";
const char * __version = "0.1";
const char * __authors = "Jones XiangTao";

static void usage(void)
{
	printf("\nbwt version %s\n", __version);
	printf("Copyright (C) 2016 %s\n", __authors);
	printf("\nUsage: %s <command> <file> [command] [eof_index]\n", __progname);
	printf("\n<Commands>\n");
	printf(" -t  Transfrom file\n");
	printf(" -r  Reverse file\n");
	printf(" -i  The index of eof char of original file in the reverse file\n");
	printf(" -h  Show this help message\n");
}

int main(int argc, char* argv[])
{
	int i;
	int tflag = 0;
	int rflag = 0;
	int iflag = 0;
	int hflag = 0;
	int eof_index = -1;
	char * fin_name = NULL;
	while ((i = getopt(argc, argv, "t:r:i:h")) != -1) {
		switch(i) {
			case 't':
				tflag = 1;
                fin_name = optarg;
				break;

			case 'r':
				rflag = 1;
                fin_name = optarg;
				break;

			case 'i':
				iflag = 1;
				eof_index = atoi(optarg);
				break;

			default:
				usage();
				exit(1);
		}
	}

	if (tflag ^ rflag && fin_name != NULL) {
        int len;
        char * buf = read_file(fin_name, &len);
        char fout_name[100];
		if (tflag) {
			int eof_index = bwt_transform(buf, len);
			sprintf(fout_name, "%s.%d.bwt", fin_name, eof_index);
		} else if (eof_index > -1) {
			bwt_reverse(buf, len, eof_index);
			sprintf(fout_name, "%s.txt", fin_name);
		} else {
			usage();
			free(buf);
			exit(1);
		}
		FILE * fout = open_file(fout_name, "w");
		fwrite(buf, sizeof(char), len, fout);
		fclose(fout);
		free(buf);
	} else {
		usage();
		exit(1);
	}
	return 0;
}
