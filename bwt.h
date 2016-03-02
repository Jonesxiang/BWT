#ifndef _BWT_H
#define _BWT_H

int bwt_transform(char * src, int len);

void bwt_reverse(char * src, int len, int eof_index);

#endif /* _BWT_H */
