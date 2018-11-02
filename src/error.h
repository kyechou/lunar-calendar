#ifndef _ERROR_H
#define _ERROR_H

extern void lc_error(const char *format, ...);
extern const char *lc_strerror(int errnum);

#endif
