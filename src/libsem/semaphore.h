#ifndef __MINIX_SEM__
#define __MINIX_SEM__

#ifdef _LINUX__
#define _ANSI_H 
#define _PROTOTYPE(x,y) x y;
#endif

#ifndef _ANSI_H
#include <ansi.h>
#endif

#include <limits.h>

#define SEM_FAILED NULL
#define SEM_VALUE_MAX 35000
typedef int sem_t;

_PROTOTYPE( int sem_close, 	(sem_t *sem)				);
_PROTOTYPE( int sem_getvalue, 	(sem_t *sem, int *val)			);
_PROTOTYPE( sem_t *sem_open, 	(const char *name, int oflag, ...)	);
_PROTOTYPE( int sem_post, 	(sem_t *sem)				);
_PROTOTYPE( int sem_trywait, 	(sem_t *sem)				);
_PROTOTYPE( int sem_unlink, 	(const char *name)			);
_PROTOTYPE( int sem_wait, 	(sem_t *sem)				);
_PROTOTYPE( int sem_init, 	(sem_t *sem, int pshared, unsigned init));
_PROTOTYPE( int sem_destroy, 	(sem_t *sem)				);

/*int    sem_timedwait(sem_t *, const struct timespec *);*/

#endif
