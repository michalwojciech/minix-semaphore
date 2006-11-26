.sect .text
.extern	__sem_unlink
.define	_sem_unlink
.align 2

_sem_unlink:
	jmp	__sem_unlink
