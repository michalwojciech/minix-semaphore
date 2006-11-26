.sect .text
.extern	__sem_init
.define	_sem_init
.align 2

_sem_init:
	jmp	__sem_init
