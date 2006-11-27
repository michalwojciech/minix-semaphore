#!/bin/bash
rm -f prod_cons
rm -f prod_cons_init
rm -f barbero
rm -f sem_serv
gcc -g -o prod_cons ../prod_cons.c ../../fs/list.c timeutils.c sem_cli.c protocolo_sem.c libconnection.a -I ../../fs/ -I ../../libsem/ -D_LINUX__
gcc -g -o prod_cons_init ../prod_cons_init.c ../../fs/list.c timeutils.c sem_cli.c protocolo_sem.c libconnection.a -I ../../fs/ -I ../../libsem/ -D_LINUX__
gcc -g -o barbero ../barbero.c ../../fs/list.c timeutils.c sem_cli.c protocolo_sem.c libconnection.a -I ../../fs/ -I ../../libsem/ -D_LINUX__
gcc -g -o sem_serv sem_serv.c ../../fs/list.c ../../fs/hash.c timeutils.c protocolo_sem.c libconnection.a ../../fs/semaphore.c -I ../../fs/ -I ../../libsem/ -lm -D_LINUX__
