#ifndef _JULIAN_H
#define _JULIAN_H

typedef struct {
	int year;
	int month;
	double day;
} GregorianDate;

extern double g2jd(int year, int month, double day);
extern GregorianDate jd2g(double jd);
/*extern double jdptime(char *isodt, char *fmt, double tz, int isut);*/
extern double jdptime(char *isodt, char *fmt);
extern size_t jdftime(char *isodt, double jd, char *fmt, double tz, int isut);
extern double deltaT(int year, int month);

#endif
