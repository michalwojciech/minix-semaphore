/* TCPechod.c - main, TCPechod */
#define MAX(x, y)       ((x) > (y) ? (x) : (y))

#include "semaphore.h"
#include "protocolo_sem.h" 
#include <fcntl.h>

#define	_USE_BSD
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define	QLEN		  32	/* maximum connection queue length	*/
#define	BUFSIZE		4096
#include "connection.h"

char nombre[255];

int	aceptar_pedido(connectionADT conn, int);
void accept_connection(connectionADT conn);

#define TRUE 1
#define FALSE 0
int who = 1;
int dont_reply = FALSE;

#define MAX_CON 31
connectionADT 	conn_array[MAX_CON];
int 		status_array[MAX_CON];

int nfds; 
fd_set afds;
#define DISCONNECTED -4
#define ERROR -1

int
main(int argc, char *argv[])
{
	char	*service = "2313";	/* service name or port number	*/
        fd_set rfds;
	int i;
	signal(SIGPIPE, SIG_IGN);
	for (i = 0; i < MAX_CON ; i++)
	{
		status_array[i] = 0;
	}
	
	connectionADT conn;

	init_sem_server();
	conn = connection_passivesock(service,"tcp");
	if ( conn == NULL )
	{
		printf("No se puede hacer la conexion\n");
		exit(1);
	}
	printf("Escuchando con el socket %d\n",connection_get_socket(conn));


        /* Limpio el conjunto de file descriptors que usará select */
	nfds = connection_get_socket(conn) + 1;
        FD_ZERO(&afds);
	while (1) {
               		FD_SET(connection_get_socket(conn), &afds);
			for ( i=0; i < MAX_CON ; i++ )
				if ( status_array[i] == 1 )
					FD_SET(connection_get_socket(conn_array[i]), &afds);
			
			memcpy(&rfds, &afds, sizeof(rfds));
			select(nfds, &rfds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);
			
                        if (FD_ISSET(connection_get_socket(conn), &rfds))
                        {
				accept_connection(conn);
			}
			else
			{
	                 	for ( i=0; i<MAX_CON; i++)
					if ( status_array[i] == 1 )
						if (FD_ISSET(connection_get_socket(conn_array[i]), &rfds))
        	                		{
							if (aceptar_pedido(conn_array[i],i) == DISCONNECTED )
							{
								status_array[i] = 0;
								printf("%d DISCONECTED!!!!!!\n", i);
							}
							break;
						}
				if ( i == MAX_CON )
					printf("CHAN!\n");
			}
	}
}

void accept_connection(connectionADT conn)
{
	int i;
	struct	sockaddr_in fsin;	/* the address of a client	*/
	unsigned int	alen;		/* length of client's address	*/

	// Busco un slot vacio
	for ( i = 0; i< MAX_CON; i++)
		if ( status_array[i] == 0 )
			break;
	if ( i == MAX_CON )
	{
		printf("No quedan slots para aceptar la conexion\n");
		return;
	}
	
	conn_array[i] = connection_accept(conn, (struct sockaddr *)&fsin, &alen);
	printf("Aceptando la conexion en el socket %d\n",connection_get_socket(conn_array[i]));
	if ( conn_array[i] == NULL )	
	{
		printf("No se puede hacer accept 1\n");
		return;
	}
	status_array[i] = 1;
	nfds = connection_get_socket(conn);	
	
	for ( i = 0; i < MAX_CON; i++ )
	{
		if ( status_array[i] == 1 )
		{
			nfds = MAX(nfds, connection_get_socket(conn_array[i]));
		}
	}
	nfds++;
        FD_ZERO(&afds);

}

void _sem_wake(connectionADT conn, int st)
{
	char buf = st;
	if (connection_send(conn, &buf, 1) < 0)
	{
		printf("No se puede escribir\n");
		return;
	}

}

void
sem_wake(int proc, int st)
{
	printf("SEM_WAKE: proc = %d\n", proc);
	_sem_wake(conn_array[proc], st);
}

int
aceptar_pedido(connectionADT conn, int proc)
{
	int	cc;
	int comando, len, rta, handle;
	int *rta_ref;
	long mode, value;

	dont_reply = FALSE;
	clear_buffer();

	printf("----------------------\n");
	printf("Pedido de parte de %d\n",proc);
	who = proc;
	//cc = connection_recv(conn, buf, sizeof buf);
	cc = connection_recv(conn, get_buffer(), MAX_MSG); 
	if (cc < 0)
	{
		printf("No se puede leer\n");
		return 0;
	}
	
	comando = get_char();
	switch (comando)
	{
		case OPEN:
			len = get_string(nombre, 255);
			
			int oflags = get_long();
			if (get_char())
			{
				mode = get_long();
				value = get_long();
				rta_ref = sem_open(nombre, oflags, mode, value);
			}
			else
			{
				rta_ref = sem_open(nombre, oflags);
			}
			if ( rta_ref == NULL )
			{
				printf("Error en el open %s\n", strerror(errno));
				rta = -1;
			}
			else
				rta = *rta_ref;
			printf("OPEN, nombre = %s oflags = %d,  value = %d \n", nombre, oflags, value);
	clear_buffer();
			break;

		case UNLINK:
	                get_string(nombre, 255);
			printf("UNLINK, nombre = %s, who = %d\n", nombre, who);
	                rta = sem_unlink(nombre);
	clear_buffer();
	                break;			

		case CLOSE:
	                handle = get_long();
			printf("CLOSE, handle = %d, who = %d\n", handle, who);
	                rta = sem_close(&handle);
	clear_buffer();
	                break;			

		case WAIT:
	                handle = get_long();
			printf("WAIT, handle = %d, who = %d\n", handle, who);
	                rta = sem_wait(&handle);
	clear_buffer();
	                break;			
		case POST:
	                handle = get_long();
			printf("POST, handle = %d, who = %d\n", handle, who);
	                rta = sem_post(&handle);
	clear_buffer();
			break;
		case TRYWAIT:
	                handle = get_long();
			printf("TRYWAIT, handle = %d, who = %d\n", handle, who);
	                rta = sem_trywait(&handle);
	clear_buffer();
			break;
		case INIT:
	                mode = get_long();
			value = get_long();
	                rta = sem_init(&handle, mode, value );
			printf("INIT, mode = %d, value = %d, who = %d\n", mode, value, who);
//			rta = handle;
	clear_buffer();
			put_long(handle);
			break;

		case GETVALUE:
	                handle = get_long();
	                rta = sem_getvalue(&handle, (int *)&value );
			printf("GETVALUE, handle = %d, value = %d, who = %d\n", handle, value, who);
//			rta = value;
	clear_buffer();
			put_long(value);
			break;

		case DESTROY:
	                handle = get_long();
			printf("DESTROY, handle = %d, who = %d\n", handle, who);
	                rta = sem_destroy(&handle);
	clear_buffer();
			break;
			
		case DISCONNECT:
		default:
			connection_destroy(&conn);
			printf("DISCONECTED\n");
			return DISCONNECTED;
			break;
	
	}
	printf("dont_reply = %d\n", dont_reply);	
	if ( dont_reply == FALSE )
	{	
		put_long(rta);
		put_long(errno);
		if (connection_send(conn, get_buffer(), get_buffer_size()) < 0)
		{
			printf("No se puede escribir\n");
			return ERROR;
		}
	}
	return 0;
}

