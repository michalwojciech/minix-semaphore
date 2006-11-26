#include <semaphore.h>
#include <lib.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

/* para O_CREAT */
#include <fcntl.h>

#define sem_close _sem_close
#define sem_getvalue _sem_getvalue
#define sem_open _sem_open
#define sem_post _sem_post
#define sem_trywait _sem_trywait
#define sem_unlink _sem_unlink
#define sem_wait _sem_wait
#define sem_init _sem_init
#define sem_destroy _sem_destroy

#define SEM_OPEN	1
#define SEM_CLOSE	2
#define SEM_INIT	3
#define SEM_WAIT	4
#define SEM_POST	5
#define SEM_ULINK	6
#define SEM_GETVAL	7
#define SEM_TRYWAIT	8
#define SEM_DESTROY	9
#define SEM_TIMEDWAIT	10



#ifdef DEBUG
#include <stdio.h>
#endif

PUBLIC int    
sem_close(sem_t *sem)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_CLOSE;
    msg.m2_i2 = *(int*)sem;
/*  msg.m2_i3
    msg.m2_l1
    msg.m2_l2
    msg.m2_p1 
*/
#ifdef DEBUG
printf("al syscall close le mando %i, %i\n", msg.m2_i1, msg.m2_i2);
#endif
    rta = _syscall(FS,SEMAPH,&msg);

#ifdef DEBUG
printf("y me retorna %i, %i\n", msg.m1_i1, msg.m2_i2);
#endif
    if (rta == 0) /* si la llamada fue exitosa */
    {
	*sem = (sem_t)msg.m1_i2; /* ponemos en sem el valor que nos mandan */
    	return msg.m1_i1; /* retornamos el valor de la función nos manda */
    }

    return -1;
}

PUBLIC int    
sem_getvalue(sem_t *sem, int *val)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_GETVAL;
    msg.m2_i2 = *(int*)sem;
    msg.m2_i3 = *val;
/*  msg.m2_l1
    msg.m2_l2
    msg.m2_p1 
*/
#ifdef DEBUG
printf("al syscall get le mando %i, %i, %i\n", msg.m2_i1, msg.m2_i2, msg.m2_i3);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i, %i, %i\n", msg.m1_i1, msg.m2_i2, msg.m2_i3);
#endif
    if (rta == 0) /* si la llamada fue exitosa */
    {
	    *val = msg.m1_i3;
	    *sem = (sem_t)msg.m1_i2;
	    return msg.m1_i1;
    }

}

PUBLIC sem_t *
sem_open(const char *name, int oflags, ...)
{
    int rta;
    sem_t *semaforo;
    message msg;
    va_list ap;

    if ((semaforo = malloc(sizeof(sem_t))) == NULL)
    {
	    errno = ENOSPC;
	    return SEM_FAILED;
    }


    msg.m2_i1 = SEM_OPEN;
    msg.m2_i3 = 2;
    msg.m2_l2 = oflags;
    msg.m2_p1 = (char*)name;

    if ( oflags == O_CREAT ) /* tenemos más parámetros si O_CREAT es la única seteada */
    {
        va_start(ap, oflags);
	msg.m2_i2 = va_arg(ap, int);
	msg.m2_l1 = va_arg(ap, int);
	msg.m2_i3 = 4;
    }

#ifdef DEBUG
printf("al syscall open le mando %s, %i, %i\n", msg.m2_p1, msg.m2_l2, msg.m2_i3);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i\n", msg.m1_i1);
#endif
    if (rta == 0)
    {
	*semaforo = (sem_t)msg.m1_i1;
	return semaforo;

    }
    return SEM_FAILED;

}

PUBLIC int    
sem_post(sem_t *sem)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_POST;
    msg.m2_i2 = *(int*)sem;
/*  msg.m2_i3
    msg.m2_l1
    msg.m2_l2
    msg.m2_p1 
*/
#ifdef DEBUG
printf("al syscall post le mando %i, %i\n", msg.m2_i1, msg.m2_i2);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i, %i\n", msg.m1_i1, msg.m2_i2);
#endif
    if (rta == 0) /* si la llamada fue exitosa */
    {
	*sem = (sem_t)msg.m1_i2; /* ponemos en sem el valor que nos mandan */
    	return msg.m1_i1; /* retornamos el valor de la función nos manda */
    }

    return -1;
}

/*
PUBLIC int    
sem_timedwait(sem_t *sem, const struct timespec *nombre??????)
{

    message msg;

    msg.m2_i1 = SEM_TIMEDWAIT;
    msg.m2_i2 = *(int*)sem;
    msg.m2_i3 
    msg.m2_l1
    msg.m2_l2
    msg.m2_p1 

    rta = _syscall(FS,SEMAPH,&msg);

    if (rta == 0) // si la llamada fue exitosa 
    {
	*sem = (sem_t)msg.m1_i2; // ponemos en sem el valor que nos mandan 
    	return msg.m1_i1; // retornamos el valor de la función nos manda 
    }

    return -1;


}
*/

PUBLIC int    
sem_trywait(sem_t *sem)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_TRYWAIT;
    msg.m2_i2 = *(int*)sem;
/*  msg.m2_i3
    msg.m2_l1
    msg.m2_l2
    msg.m2_p1 
*/
#ifdef DEBUG
printf("al syscall try le mando %i, %i\n", msg.m2_i1, msg.m2_i2);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i, %i\n", msg.m1_i1, msg.m2_i2);
#endif
    if (rta == 0) /* si la llamada fue exitosa */
    {
	*sem = (sem_t)msg.m1_i2; /* ponemos en sem el valor que nos mandan */
    	return msg.m1_i1; /* retornamos el valor de la función nos manda */
    }

    return -1;
}


PUBLIC int    
sem_unlink(const char *name)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_ULINK;
/*  msg.m2_i2 = *sem;
    msg.m2_i3
    msg.m2_l1
    msg.m2_l2
*/  msg.m2_p1 = (char*)name;
#ifdef DEBUG
printf("al syscall unlink le mando %i, %s\n", msg.m2_i1, msg.m2_p1);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i\n", msg.m1_i1);
#endif
    if (rta == 0)
	    return msg.m1_i1;
    return -1;
}


PUBLIC int
sem_wait(sem_t *sem)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_WAIT;
    msg.m2_i2 = *(int*)sem;
/*  msg.m2_i3
    msg.m2_l1
    msg.m2_l2
    msg.m2_p1 
*/
#ifdef DEBUG
printf("al syscall wait le mando %i, %i\n", msg.m2_i1, msg.m2_i2);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i\n", msg.m1_i1, msg.m2_i2);
#endif
    if (rta == 0) /* si la llamada fue exitosa */
    {
	*sem = (sem_t)msg.m1_i2; /* ponemos en sem el valor que nos mandan */
    	return msg.m1_i1; /* retornamos el valor de la función nos manda */
    }

    return -1;
}


PUBLIC int    
sem_init(sem_t *sem, int pshared, unsigned init)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_INIT;
    msg.m2_i2 = *(int*)sem;
    msg.m2_i3 = pshared;
    msg.m2_l1 = init;
/*  msg.m2_l2
    msg.m2_p1 
*/
#ifdef DEBUG
printf("al syscall init le mando %i, %i, %i, %i\n", msg.m2_i1, msg.m2_i2, msg.m2_i3, msg.m2_l1);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i, %i\n", msg.m1_i1, msg.m2_i2);
#endif
    if (rta == 0) /* si la llamada fue exitosa */
    {
	*sem = (sem_t)msg.m1_i2; /* ponemos en sem el valor que nos mandan */
    	return msg.m1_i1; /* retornamos el valor de la función nos manda */
    }

    return -1;
}


PUBLIC int    
sem_destroy(sem_t *sem)
{
    int rta;
    message msg;

    msg.m2_i1 = SEM_DESTROY;
    msg.m2_i2 = *(int*)sem;
/*  msg.m2_i3
    msg.m2_l1
    msg.m2_l2
    msg.m2_p1 
*/
#ifdef DEBUG
printf("al syscall dest le mando %i, %i\n", msg.m2_i1, msg.m2_i2);
#endif
    rta = _syscall(FS,SEMAPH,&msg);
#ifdef DEBUG
printf("y me retorna %i, %i\n", msg.m1_i1, msg.m2_i2);
#endif
    if (rta == 0) /* si la llamada fue exitosa */
    {
	*sem = (sem_t)msg.m1_i2; /* ponemos en sem el valor que nos mandan */
    	return msg.m1_i1; /* retornamos el valor de la función nos manda */
    }

    return -1;
}

