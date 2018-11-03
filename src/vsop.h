#ifndef _VSOP_H
#define _VSOP_H

extern double vsopLx(double vsopterms[][3], size_t rowcount, double t);
extern double vsop(double jd);
extern double apparentsun(double jd, int ignorenutation);

#endif
