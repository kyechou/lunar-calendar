#ifndef LUNAR_CALENDAR_ERROR_H
#define LUNAR_CALENDAR_ERROR_H

extern void lc_error(const char *format, ...);
extern const char *lc_strerror(int errnum);

#endif
