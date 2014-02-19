#include "../h/pcb.h"
//#include "../h/asl.h"
#include "../h/const.h"

typedef struct semd_t {
        struct semd_t *s_next; /* next element on the ASL */
        int *s_semAdd;         /* pointer to the semaphore */
        pcb_t *s_procQ;        /*tail pointer to a process queue */
} semd_t;

semd_t *semdFree, *semdFree_h, *semd_h;
static semd_t semdTable[MAXPROC+1];

void initASL()
{
	int i;

	semd_h = &semdTable[0]; // dummy
	semd_h->s_next = NULL; // dummy

	for(i=1; i<MAXPROC+1; i++)
	{
		semdFree = &semdTable[i];

		if(i == 1)
			semdFree_h = semdFree;

		if(i == MAXPROC)
			semdFree->s_next = NULL;

		else semdFree->s_next = &semdTable[i+1];
	}
}


//look4sema4 cerca il semafoto nella lista semd_h, se lo trova lo restituisce
//altrimenti restituisce NULL
semd_t *look4sema4(int *semAdd)
{
	semd_t *semdTmp = semd_h;

	while(*(semdTmp->s_semAdd) != *semAdd && semdTmp != NULL)
		semdTmp = semdTmp->s_next;

	return semdTmp;
}

int insertBlocked(int *semAdd, pcb_t *p)
{
	semd_t *semdTmp, *semdList = semd_h;

	*(p->p_semAdd) = *semAdd;

	if((semdTmp = look4sema4(semAdd)) != NULL)
		insertProcQ(&(semdTmp->s_procQ), p);

	else
	{
		if(semdFree_h == NULL)
			return TRUE;

		semdTmp = semdFree_h;
		semdFree_h = semdFree_h->s_next;

		while((semdList->s_next) != NULL)
		{
			if(*semAdd < *((semdList->s_next)->s_semAdd))
				break;
			semdList = semdList->s_next;

		}

		semdTmp->s_next = semdList->s_next;
		semdList->s_next = semdTmp;

		semdTmp->s_procQ = mkEmptyProcQ();
		insertProcQ(&(semdTmp->s_procQ), p);

		return FALSE;
	}
}
