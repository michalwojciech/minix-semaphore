#!/bin/bash
rm -f prod_cons
rm -f prod_cons_init
rm -f barbero
rm -f sem_serv
gcc -g -o prod_cons ../prod_cons.c ../../fs/list.c timeutils.c sem_cli.c protocolo_sem.c libconnection.a -I ../../
gcc -g -o prod_cons_init ../prod_cons_init.c ../../fs/list.c timeutils.c sem_cli.c protocolo_sem.c libconnection.a -I ../../
gcc -g -o barbero ../barbero.c ../../fs/list.c timeutils.c sem_cli.c protocolo_sem.c libconnection.a -I ../../
gcc -g -o sem_serv sem_serv.c ../../fs/list.c ../../fs/hash.c timeutils.c protocolo_sem.c libconnection.a ../../semaphore.c -I ../../ -lm -D_LINUX__
