#include "timeutils.h"
#include <stdlib.h>

struct timeval timediff(struct timeval t1, struct timeval t2)
{
        t1.tv_sec -=t2.tv_sec;
        t1.tv_usec -=t2.tv_usec;
        if ( t1.tv_usec < 0 )
        {       t1.tv_sec--;
                t1.tv_usec += 1000000;
        }

        return t1;
}

int timecmp(struct timeval t1, struct timeval t2)
{
        if ( t1.tv_sec < t2.tv_sec )
                return -1;

        if ( t1.tv_sec > t2.tv_sec )
                return 1;

        if ( t1.tv_usec < t2.tv_usec )
                return -1;

        if ( t1.tv_usec > t2.tv_usec )
                return 1;
        return 0;
}

struct timeval addDelay(struct timeval time, int sec, int usec)
{
        time.tv_sec += sec;
        time.tv_usec += usec;
        if (time.tv_usec >= 1000000 )
        {
                time.tv_usec -= 1000000;
                time.tv_sec++;
        }
        return time;
}

struct timeval getDelay(int sec, int usec)
{
        struct timeval now;
        gettimeofday(&now, NULL);
        now = addDelay(now,sec,usec);
        return now;
}
