#include <stdio.h>
#include <errno.h>
#include "error.h"
#include "md5.h"
#include "md5sum.h"

int md5sum(unsigned char *digest, const char *filename)
{
	FILE *file;
	MD5_CTX ctx;
	size_t size;
	unsigned char data[1024];

	if ((file = fopen(filename, "rb")) == NULL) {
		lc_error("failed to open %s: %s\n", filename, lc_strerror(errno));
		return -1;
	}

	MD5_Init(&ctx);
	while ((size = fread(data, 1, 1024, file)) != 0)
		MD5_Update(&ctx, data, size);
	MD5_Final(digest, &ctx);

	if (fclose(file) == EOF) {
		lc_error("failed to close %s: %s\n", filename, lc_strerror(errno));
		return -1;
	}
	return 0;
}

int md5cmp(const unsigned char *a, const unsigned char *b)
{
	for (int i = 0; i < 16; ++i) {
		if (a[i] > b[i])
			return 1;
		else if (a[i] < b[i])
			return -1;
	}
	return 0;
}
