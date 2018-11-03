#ifndef _LEA406_H
#define _LEA406_H

struct worker_param {
	int tid;
	double tc;           /* t in century */
};

extern double apparentmoon(double jd, int ignorenutation);
extern double lea406(double jd, int ignorenutation);

#endif
