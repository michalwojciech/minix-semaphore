#ifndef TIMEFUNS_H_
#define TIMEFUNS_H_
#include <sys/time.h>

struct timeval timediff(struct timeval t1, struct timeval t2);
int timecmp(struct timeval t1, struct timeval t2);
struct timeval addDelay(struct timeval time, int sec, int usec);
struct timeval getDelay(int sec, int usec);
#endif /*TIMEFUNS_H_*/
