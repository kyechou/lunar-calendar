#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "lunarcalbase.h"

static const struct option longopts[] = {
	{"start", required_argument, NULL, 's'},
	{"end", required_argument, NULL, 'e'},
	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0}
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

int main(int argc, char **argv)
{
	int op;
	char *endptr;
	long start = 0, end = 0;

	while ((op = getopt_long(argc, argv, "s:e:h", longopts, NULL)) != -1) {
		switch (op) {
		case 's':
			start = strtol(optarg, &endptr, 10);
			if (*optarg == '\0' || *endptr != '\0') {
				fprintf(stderr, "error: invalid start year \'%s\'\n", optarg);
				return 1;
			}
			break;
		case 'e':
			end = strtol(optarg, &endptr, 10);
			if (*optarg == '\0' || *endptr != '\0') {
				fprintf(stderr, "error: invalid end year \'%s\'\n", optarg);
				return 1;
			}
			break;
		case 'h':
			fputs(usage, stdout);
			return 0;
		case '?':
		default:
			fputs(usage, stderr);
			return 1;
		}
	}

	if (start == 0)
		start = 777;
	if (end == 0)
		end = start;

	/*
	printf("BEGIN:VCALENDAR\n"
	       "VERSION:2.0\n"
	       "PRODID:-//KY Chou//Lunar Calendar//ZH\n"
	       "CALSCALE:GREGORIAN\n"
	       "METHOD:PUBLISH\n"
	       "X-WR-CALNAME:農曆\n"
	       "X-WR-TIMEZONE:Asia/Taipei\n"
	       "X-WR-CALDESC:農曆%d-%d, 包括节气.\n", start, end);

	while (start <= end) {
		cn_lunarcal(start);
		++start;
	}

	printf("END:VCALENDAR\n");
	*/

	return 0;
}
