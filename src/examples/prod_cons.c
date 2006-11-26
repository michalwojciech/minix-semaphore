#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.h"
#include <sys/types.h>
#include <fcntl.h>

void productor(void);
void consumidor(void);

int
main(int argc, char *argv[])
{
	
	if (*argv[1] == 'p' )
		productor();
	else
		consumidor();
	return 0;
}

void productor(void)
{
	int c;
	sem_t *sem_prod, *sem_cons;

	printf("PRODUCTOR\n");
	sem_prod = sem_open("sem_prod",O_CREAT, 0, 15 );
	sem_cons = sem_open("sem_cons",O_CREAT, 0, 0);
	printf ("sem_prod = %p, *sem_prod=%d\n", sem_prod, *sem_prod);
	printf ("sem_cons = %p, *sem_cons=%d\n", sem_cons, *sem_cons);

       c = 1;
        while (c != EOF)
        {
                sem_wait(sem_prod);
                c = getchar();
                if (c!=EOF)
                {
			printf("Creo elemento\n");
                        sem_post(sem_cons);
                }
        }
	sem_unlink("sem_cons");
	sem_unlink("sem_prod");

	sem_close(sem_cons);
	sem_close(sem_prod);

}
void consumidor()
{
	int c;
	sem_t *sem_prod, *sem_cons;

	printf("CONSUMIDOR\n");
        sem_prod = sem_open("sem_prod",O_CREAT, 0, 15);
        sem_cons = sem_open("sem_cons",O_CREAT, 0, 0 );
	printf ("sem_prod = %p, *sem_prod=%d\n", sem_prod, *sem_prod);
	printf ("sem_cons = %p, *sem_cons=%d\n", sem_cons, *sem_cons);

       while (c != EOF)
        {
                c = getchar();
                if (c!=EOF)
		{
	                sem_wait(sem_cons);
			printf("Consumo\n");
		}
                sem_post(sem_prod);
        }
	sem_unlink("sem_cons");
	sem_unlink("sem_prod");

	sem_close(sem_cons);
	sem_close(sem_prod);

}

