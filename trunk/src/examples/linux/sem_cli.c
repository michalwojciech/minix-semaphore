#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "connection.h"
#include "semaphore.h"
#include "protocolo_sem.h"
#include <stdarg.h>
#include <fcntl.h>
connectionADT conn;


void clean_all(void)
{
	clear_buffer();
	put_char(DISCONNECT);

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}
	connection_destroy(&conn);
}
sem_t * 
sem_open(const char *nombre, int oflags, ...)
{
	int cc;
	char name_l;
	int * rta;
	va_list ap;

	// Si no existe una conexion la creo.
	if ( conn == NULL )
	{
		char	*service = "2313";	/* service name or port number	*/
		atexit(clean_all);	
		conn = connection_connect_to("localhost", service, "tcp");
		if (conn == NULL)
		{
			printf("No me pude conectar, aborto.\n");
			exit (1);
		}
	}
	
	clear_buffer();
	va_start (ap, oflags);
	put_char(OPEN);
	name_l = strlen(nombre);
	put_string((char *)nombre, name_l);
	put_long(oflags);
	if ( oflags & O_CREAT )
	{
		put_char(1);
		put_long(va_arg(ap, mode_t));	
		put_long(va_arg(ap, int));
	}
	else
		put_char(0);

	va_end (ap);
	
	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	rta = malloc(sizeof(int));
//	*rta = buf[0];
	*rta = (sem_t) get_long();
	return rta;
}
int sem_post(sem_t * handle)
{
	int cc;
	clear_buffer();
	put_char(POST);
	put_long(*handle);
	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	return (int) get_long();

}
int sem_wait(sem_t * handle)
{
	int cc;
	clear_buffer();
	put_char(WAIT);
	put_long(*handle);

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	return get_long();
}

int
sem_close(sem_t * handle)
{
	int cc;
	clear_buffer();
	put_char(CLOSE);
	put_long(*handle);

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	return get_long();
}		

int    
sem_trywait(sem_t * handle)
{
	int cc;
	clear_buffer();
	put_char(TRYWAIT);
	put_long(*handle);

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	return get_long();
}

int
sem_unlink(const char * name)
{
	int cc;
	clear_buffer();
	put_char(UNLINK);
	put_string((char*)name, strlen(name));

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	return get_long();
}

int
sem_init (sem_t *sem, int pshared, unsigned value)
{
	int cc;
	clear_buffer();
	put_char(INIT);
	put_long(pshared);
	put_long(value);

	// Si no existe una conexion la creo.
	if ( conn == NULL )
	{
		char	*service = "2313";	/* service name or port number	*/
		atexit(clean_all);	
		conn = connection_connect_to("localhost", service, "tcp");
		if (conn == NULL)
		{
			printf("No me pude conectar, aborto.\n");
			exit (1);
		}
	}

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	*sem = (sem_t) get_long();
	return get_long();
}

int    
sem_destroy(sem_t * handle)
{
	int cc;
	clear_buffer();
	put_char(DESTROY);
	put_long(*handle);

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return -1;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return -1;
	}
	return get_long();
}

int    
sem_getvalue(sem_t * handle, int * value)
{
	int cc;
	clear_buffer();
	put_char(GETVALUE);
	put_long(*handle);

	if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

	clear_buffer();
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return;
	}
	*value =  get_long();
	return get_long();
}
