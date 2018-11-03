#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include <fts.h>
#include <errno.h>
#include <ctype.h>
#include "error.h"
#include "md5sum.h"
#include "tz.h"

static void trim(char *line)
{
	int start, end, len = strlen(line);

	for (start = 0; start < len && isspace((line)[start]); ++start);
	for (end = len - 1; end >= 0 && isspace((line)[end]); --end);
	len = (end < start) ? 0 : end - start + 1;
	if (start > 0) {
		for (int i = 0; i < len; ++i)
			line[i] = line[start + i];
	}
	line[len] = 0;
}

static int read_from_etc_timezone(char *buf, size_t buflen)
{
	size_t count;
	FILE *file = fopen("/etc/timezone", "r");
	if (file == NULL) {
		lc_error("failed to open /etc/timezone: %s\n",
		         lc_strerror(errno));
		return -1;
	}
	count = fread(buf, sizeof(char), buflen, file);
	buf[count] = 0;
	trim(buf);
	if (fclose(file) == EOF) {
		lc_error("failed to close /etc/timezone: %s\n",
		         lc_strerror(errno));
		return -1;
	}
	return 0;
}

static int read_from_localtime_slink(char *buf, size_t buflen)
{
	ssize_t count;
	char lnbuf[512], *path, *area, *location;

	count = readlink("/etc/localtime", lnbuf, sizeof(lnbuf));
	if (count == -1) {
		lc_error("readlink failed: %s\n", lc_strerror(errno));
		return -1;
	}
	if (count >= (ssize_t)sizeof(lnbuf)) {
		lc_error("tz link path too long\n");
		return -1;
	}
	lnbuf[count] = 0;
	path = strndup(lnbuf, count);
	location = basename(lnbuf);
	area = basename(dirname(path));
	if (strlen(area) + strlen(location) + 1 > buflen - 1) {
		lc_error("timezone buffer is not big enough\n");
		return -1;
	}
	strncpy(buf, area, buflen);
	strncat(buf, "/", buflen - strlen(area));
	strncat(buf, location, buflen - strlen(area) - 1);
	free(path);
	return 0;
}

static int compare_tz_files(char *buf, size_t buflen)
{
	FTS *fs;
	FTSENT *file;
	unsigned char orig_digest[16], digest[16];
	char *const paths[] = { "/usr/share/zoneinfo/", NULL };
	char *area, *location;

	if (md5sum(orig_digest, "/etc/localtime") < 0)
		return -1;

	fs = fts_open(paths, FTS_COMFOLLOW | FTS_LOGICAL | FTS_NOCHDIR, NULL);
	if (fs == NULL) {
		lc_error("fts_open failed: %s\n", lc_strerror(errno));
		return -1;
	}

	while ((file = fts_read(fs))) {
		if (file->fts_info == FTS_F) {
			if (md5sum(digest, file->fts_path) < 0)
				return -1;
			if (md5cmp(orig_digest, digest) == 0) {
				area = basename(dirname(file->fts_path));
				location = file->fts_name;
				strncpy(buf, area, buflen);
				strncat(buf, "/", buflen - strlen(area));
				strncat(buf, location, buflen - strlen(area) - 1);
			}
		}
	}
	if (errno) {
		lc_error("fts_read failed: %s\n", lc_strerror(errno));
		return -1;
	}

	return 0;
}

int current_tz(char *buf, size_t buflen)
{
	struct stat file_st;

	if (stat("/etc/timezone", &file_st) == 0 && S_ISREG(file_st.st_mode))
		return read_from_etc_timezone(buf, buflen);

	if (lstat("/etc/localtime", &file_st) == -1) {
		lc_error("lstat /etc/localtime failed: %s\n", lc_strerror(errno));
		return -1;
	}

	if (S_ISLNK(file_st.st_mode))
		return read_from_localtime_slink(buf, buflen);

	if (S_ISREG(file_st.st_mode))
		return compare_tz_files(buf, buflen);

	/* unable to determine the system tz */
	strncpy(buf, "Asia/Taipei", buflen - 1);
	buf[buflen] = 0;
	return 0;
}
