#ifndef _MD5SUM_H
#define _MD5SUM_H

extern int md5sum(unsigned char *digest, const char *filename);
extern int md5cmp(const unsigned char *a, const unsigned char *b);

#endif
