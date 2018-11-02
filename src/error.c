#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <locale.h>
#include "error.h"

void lc_error(const char *format, ...)
{
	va_list arglist;

	fflush(stdout);
	fputs("lunar-calendar: ", stderr);
	va_start(arglist, format);
	vfprintf(stderr, format, arglist);
	va_end(arglist);
}

const char *lc_strerror(int errnum)
{
	const char *msg;
	locale_t locale;

	locale = newlocale(LC_ALL_MASK, "", 0);
	msg = strerror_l(errnum, locale);
	freelocale(locale);
	return msg;
}
