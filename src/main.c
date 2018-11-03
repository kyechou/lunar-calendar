#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <errno.h>
#include "error.h"
#include "tz.h"
#include "lunarcal.h"

static const struct option longopts[] = {
	{"start", required_argument, NULL, 's'},
	{"end",   required_argument, NULL, 'e'},
	{"help",  no_argument,       NULL, 'h'},
	{ 0,      0,                 0,     0 }
};

static const char usage[] =
        "Usage: lunar-calendar [OPTION]...\n"
        "\n"
        "Output a lunar calendar in iCalendar format.\n"
        "\n"
        "Options:\n"
        "    -h, --help           print this message\n"
        "    -s, --start <year>   specify the start year (default: the current year)\n"
        "    -e, --end <year>     specify the end year (default: the start year)\n"
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

	if (current_tz(tz, sizeof(tz)) < 0) {
		lc_error("current_tz failed\n");
		return -1;
	}

	printf("BEGIN:VCALENDAR\n"
	       "VERSION:2.0\n"
	       "PRODID:-//Lunar Calendar//Lunar Calendar %ld-%ld//EN\n"
	       "CALSCALE:GREGORIAN\n"
	       "METHOD:PUBLISH\n"
	       "X-WR-CALNAME:農曆\n"
	       "X-WR-TIMEZONE:Asia/Taipei\n"
	       "X-WR-CALDESC:農曆 %ld-%ld (含節氣).\n", start, end, start, end);

	while (start <= end) {
		cn_lunarcal(start);
		++start;
	}

	printf("END:VCALENDAR\n");

	return 0;
}
