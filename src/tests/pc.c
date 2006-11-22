#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#define ITEMS 10
#define ITEM_MIN 0
#define ITEM_MAX 100
#define T_BETWEEN 2

#define FNAME "pc-test"


void producer(void);
void consumer(void);
int int_rand(int high, int low);

sem_t *s_empty;
sem_t *s_full;
sem_t *mutex;
int fd; /* File descriptor del archivo usado */

int
main(void)
{
	int pid;
	
	/* Inicializo rand */
	srand(time(NULL));

	/* Abro el archivo */
	fd = open(FNAME,
		O_RDWR | O_CREAT,
		S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);

	s_empty = sem_open("/empty", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, ITEMS);
	s_full = sem_open("/full", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 0);
	mutex = sem_open("/mutex", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, 1);

	/* Genero los procesos consumidor y productor */
	if ((pid = fork()) == -1)
		exit(EXIT_FAILURE);
	else if (pid == 0)
		/* Proceso hijo - Consumidor */
		consumer();
	else
		/* Proceso padre - Productor */
		producer();

	return 0;
}


/* Genera números al azar en un rango */
int
int_rand(int high, int low)
{
	return (rand() % (high - low + 1)) + low;
}


/* Función del proceso productor */
void
producer(void)
{
	int item;
	int items = ITEMS;
	
	/* Ciclo para escritura de cada elemento */
	while (items--)
	{
		/* Genero el ítem */
		item = int_rand(ITEM_MAX, ITEM_MIN);
		
		/* Decremento la cantidad de casilleros vacíos.
		 * Si lo logro es porque había espacio. */
		sem_wait(s_empty);

		/* Entro a la región crítica: escribo archivo */
		sem_wait(mutex);

		/* Coloco el elemento en el archivo */
		write(fd, (void*)&item, sizeof(int));
		//sync();
		//sleep(T_BETWEEN);
		printf("PRODUCTOR: Producción de %d.\n", item);

		/* Salgo de la región crítica de escritura */
		sem_post(mutex);

		/* Incremento la cantidad de casilleros llenos */
		sem_post(s_full);
	}
	
	close(fd);
	
	sem_close(s_empty);
	sem_close(s_full);
	sem_close(mutex);
}


/* Función de proceso consumidor */
void
consumer(void)
{
	int item;
	int items = ITEMS;

	/* Ciclo para lectura de cada elemento */
	while (items--)
	{
		/* Decremento la cantidad de casilleros llenos.
		 * Si lo logro es porque había algo que consumir. */
		sem_wait(s_full);

		/* Entro a la región crítica: escribo archivo */
		sem_wait(mutex);

		/* Consumo un elemento desde el archivo */
		read(fd, (void*)&item, sizeof(int));
//		sleep(T_BETWEEN);
		printf("CONSUMIDOR: Consumición de %d.\n", item);

		/* Salgo de la región crítica de escritura */
		sem_post(mutex);

		/* Incremento la cantidad de casilleros vacíos */
		sem_post(s_empty);
	}
	
	close(fd);
	
	sem_unlink("/empty");
	sem_unlink("/full");
	sem_unlink("/mutex");
	
	sem_close(s_empty);
	sem_close(s_full);
	sem_close(mutex);
}

