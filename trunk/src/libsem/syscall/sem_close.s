.sect .text
.extern	__sem_close
.define	_sem_close
.align 2

_sem_close:
	jmp	__sem_close
