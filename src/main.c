#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <libgen.h>
#include "error.h"
#include "lunarcalbase.h"

static const struct option longopts[] = {
	{"start", required_argument, NULL, 's'},
	{"end",   required_argument, NULL, 'e'},
	{"help",  no_argument,       NULL, 'h'},
	{0,       0,                 0,     0 }
};

static const char usage[] =
        "Usage: lunar-calendar [OPTION]...\n"
        "\n"
        "Output a lunar calendar in iCalendar format.\n"
        "\n"
        "Options:\n"
        "    -s, --start <year>   specify the start year (default: the current year)\n"
        "    -e, --end <year>     specify the end year (default: the start year)\n"
        "    -h, --help           print this message\n"
        "\n"
        "Note: <year> must be in the range: [1901, 2100]\n"
        "\n";

static int current_year(void)
{
	time_t now;
	struct tm tm_now;

	time(&now);
	if (gmtime_r(&now, &tm_now) == NULL) {
		lc_error("gmtime_r failed: %s\n", lc_strerror(errno));
		return -1;
	}
	return tm_now.tm_year + 1900;
}

static int get_timezone(char *buf, size_t buflen)
{
	struct stat file_st;

	if (stat("/etc/timezone", &file_st) == 0 && S_ISREG(file_st.st_mode)) {
		/* read from /etc/timezone as the Olson tz */
		FILE *file = fopen("/etc/timezone", "r");
		if (file == NULL) {
			lc_error("failed to open /etc/timezone: %s\n",
			         lc_strerror(errno));
			return -1;
		}
		if (fclose(file) == EOF) {
			lc_error("failed to close /etc/timezone: %s\n",
			         lc_strerror(errno));
			return -1;
		}
		return 0;
	}

	if (lstat("/etc/localtime", &file_st) == -1) {
		lc_error("lstat /etc/localtime failed: %s\n", lc_strerror(errno));
		return -1;
	}
	if (S_ISLNK(file_st.st_mode)) {
		/* read the symbolic link as the Olson tz */
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
	} else if (S_ISREG(file_st.st_mode)) {
		/* compare md5 digests with /usr/share/zoneinfo/... to find out
		 * the Olson tz */
		;
	} else {
		strncpy(buf, "Asia/Taipei", buflen - 1);
		buf[buflen] = 0;
	}
	return 0;
}

int main(int argc, char **argv)
{
	int op;
	char *endptr, tz[64];
	long start = 0, end = 0;

	while ((op = getopt_long(argc, argv, "s:e:h", longopts, NULL)) != -1) {
		switch (op) {
		case 's':
			start = strtol(optarg, &endptr, 10);
			if (*optarg == '\0' || *endptr != '\0') {
				lc_error("invalid start year \'%s\'\n", optarg);
				return -1;
			}
			break;
		case 'e':
			end = strtol(optarg, &endptr, 10);
			if (*optarg == '\0' || *endptr != '\0') {
				lc_error("invalid end year \'%s\'\n", optarg);
				return -1;
			}
			break;
		case 'h':
			fputs(usage, stdout);
			return 0;
		case '?':
		default:
			fputs(usage, stderr);
			return -1;
		}
	}

	if (start == 0) {
		start = current_year();
		if (start < 0)
			return -1;
	}
	if (end == 0) {
		end = start;
	} else if (end < start) {
		lc_error("the end year must be greater than or equal to the"
		         "start year\n");
		return -1;
	}

	if (get_timezone(tz, sizeof(tz)) < 0) {
		lc_error("get_timezone failed\n");
		return -1;
	}

	printf("BEGIN:VCALENDAR\n"
	       "VERSION:2.0\n"
	       "PRODID:-//KY Chou//Lunar Calendar//ZH\n"
	       "CALSCALE:GREGORIAN\n"
	       "METHOD:PUBLISH\n"
	       "X-WR-CALNAME:農曆\n"
	       "X-WR-TIMEZONE:%s\n"
	       "X-WR-CALDESC:農曆 %ld-%ld (含節氣).\n", tz, start, end);

	while (start <= end) {
		/*cn_lunarcal(start);*/
		++start;
	}

	printf("END:VCALENDAR\n");

	return 0;
}
