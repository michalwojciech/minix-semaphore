.sect .text
.extern	__sem_open
.define	_sem_open
.align 2

_sem_open:
	jmp	__sem_open
