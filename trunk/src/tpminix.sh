#!/bin/sh

case $1 in
	install)
		echo "instalando...."
#
# Primero hacemos backups
#

		mkdir /bak

		mkdir /bak/fs

		mkdir /bak/mm

		mkdir /bak/include

		mkdir /bak/include/minix

		mkdir /bak/man

		cp -f /usr/src/fs/*  /bak/fs

		cp -f /usr/src/mm/table.c  /bak/mm/

		cp -f /usr/include/minix/*.h  /bak/include/minix

		cp -f /bak/man /usr/man/whatis
#
# Después copiamos nuestros archivos
#

		cp fs/* /usr/src/fs

		cp mm/table.c /usr/src/mm

		cp include/minix/*.h /usr/include/minix


		mkdir /usr/src/lib/libsem

		mkdir /usr/src/lib/libsem/syscall

		cp libsem/syscall/* /usr/src/lib/libsem/syscall

		cp libsem/*.c /usr/src/lib/libsem

		cp libsem/semaphore.h /usr/include


		mkdir /usr/ast/sem

		cp examples/* /usr/ast/sem

		cp man/* /usr/man/man3

		echo "semaphore (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_close (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_destroy (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_getvalue (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_init (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_open (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_post (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_trywait (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_unlink (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_wait (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis

#
# Compilamos nuestra biblioteca
#

		cd /usr/src/lib/libsem
		cc -c semaphore.c
		cd /usr/src/lib/libsem/syscall
		cc -c *.s

		aal cr /usr/lib/i386/libsem.a /usr/src/lib/libsem/syscall/*.o
		aal cr /usr/lib/i386/libsem.a /usr/src/lib/libsem/semaphore.o

#
# Complamos el/los ejemplos
#
		cd /usr/ast/sem
		cc -o prod_cons prod_cons.c -l sem
		cc -o pc_init pc_init.c -l sem
		cc -o console console.c -l sem

#
# Finalmente recompilamos el kernel
#
		cd /usr/src/tools
		make
		make hdboot
		reboot

		;;
	uninstall)
		echo "desinstalando..."

		cp /bak/fs/* /usr/src/fs/ 
		cp /bak/mm/* /usr/src/mm/
		cp /bak/include/minix/* /usr/include/minix/
		cp /bak/man/whatis /usr/man/
		rm -f /usr/src/lib/libsem/syscall/*
		rmdir /usr/src/lib/libsem/syscall
		rm -f /usr/src/lib/libsem/*
		rmdir /usr/src/lib/libsem
		rm /usr/ast/sem/*
		rmdir /usr/ast/sem
		rm /usr/lib/i386/libsem.a
		rm /usr/include/semaphore.h

#
# Finalmente recompilamos el kernel
#
		cd /usr/src/tools
		make
		make hdboot
		reboot
		;;
	install2)
		echo "instalando...."
#
# Primero hacemos backups
#
		echo "creo back"
		mkdir /bak
		echo "creo fs"
		mkdir /bak/fs
		echo "creo mm"
		mkdir /bak/mm
		echo "creo include"
		mkdir /bak/include
		echo "creo minix"
		mkdir /bak/include/minix
		echo "backup de fs"
		cp -f /usr/src/fs/*  /bak/fs
		echo "backup de mm"
		cp -f /usr/src/mm/table.c  /bak/mm/
		echo "backup de include"
		cp -f /usr/include/minix/*.h  /bak/include/minix
#
# Después copiamos nuestros archivos
#
		echo "instalando en fs"
		cp fs/* /usr/src/fs
		echo "instalando en mm"
		cp mm/table.c /usr/src/mm
		echo "instalando en include"
		cp include/minix/*.h /usr/include/minix


		mkdir /usr/src/lib/libsem
		echo "creo directorio libsem/syscall"
		mkdir /usr/src/lib/libsem/syscall
		echo "instalando libsem/syscall"
		cp libsem/syscall/* /usr/src/lib/libsem/syscall
		echo "instalando libsem"
		cp libsem/*.c /usr/src/lib/libsem
		echo "instalando semaphore.h"
		cp libsem/semaphore.h /usr/include

		echo "creo directorio ast/sem"
		mkdir /usr/ast/sem
		echo "instalando examples"
		cp examples/* /usr/ast/sem

		cp man/* /usr/man/man3

		echo "semaphore (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_close (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_destroy (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_getvalue (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_init (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_open (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_post (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_trywait (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_unlink (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis
		echo "sem_wait (3) - interfaz de la biblioteca de semaforos" >> /usr/man/whatis

#
# Compilamos nuestra biblioteca
#
		echo "cambio directorio a libsem"
		cd /usr/src/lib/libsem
		echo "compilo semaphore.c"
		cc -c semaphore.c
		echo "cambio directorio a libsem/syscall"
		cd /usr/src/lib/libsem/syscall
		echo "compilando syscalls"
		cc -c *.s

		echo "aal *.s"
		aal cr /usr/lib/i386/libsem.a /usr/src/lib/libsem/syscall/*.o
		echo "aal semaphore.o"
		aal cr /usr/lib/i386/libsem.a /usr/src/lib/libsem/semaphore.o

#
# Complamos el/los ejemplos
#
		echo "cambio directorio a ast/sem"
		cd /usr/ast/sem
		echo "compilo prod_cons"
		cc -o prod_cons prod_cons.c -l sem
		echo "compilo pc_init"
		cc -o pc_init pc_init.c -l sem
		echo "compilo console"
		cc -o console console.c -l sem

#
# Finalmente recompilamos el kernel
#
		;;
	uninstall2)
		echo "desinstalando..."

		echo "restauro fs"
		cp /bak/fs/* /usr/src/fs/ 
		echo "restauro mm"
		cp /bak/mm/* /usr/src/mm/
		echo "restauro include"
		cp /bak/include/minix/* /usr/include/minix/
		echo "borro libsem/syscall/*"
		rm -f /usr/src/lib/libsem/syscall/*
		echo "borro libsem/syscall"
		rmdir /usr/src/lib/libsem/syscall
		echo "borro libsem/*"
		rm -f /usr/src/lib/libsem/*
		echo "borro libsem"
		rmdir /usr/src/lib/libsem
		echo "borro ast/sem/*"
		rm /usr/ast/sem/*
		echo "borro ast/sem"
		rmdir /usr/ast/sem
		echo "borro biblioteca"
		rm /usr/lib/i386/libsem.a
		echo "borro semaphore.h"
		rm /usr/include/semaphore.h


#
# Finalmente recompilamos el kernel
#
		;;
	ker)
		echo "cambio directorio a tools"
		cd /usr/src/tools
		echo "making"
		make
		echo "hdbooting"
		make hdboot

		;;
	*)
		echo "Uso: $0 {install|uninstall}"
		;;
esac
