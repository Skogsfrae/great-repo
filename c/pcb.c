#include "../h/const.h"
#include "../h/pcb.h"

pcb_t *pcbFree, *pcbFree_h;

void initPcbs()
{
	int i;
	extern pcb_t *procp[MAXPROC];

	pcbFree_h = procp[0];

	for(i=0; i < MAXPROC; i++)
	{
		pcbFree = procp[i];
		pcbFree->p_next = procp[i+1];
		pcbFree->p_prnt = pcbFree->p_child = pcbFree->p_sib = NULL;
		pcbFree->p_s = pcbFree->p_semAdd = NULL;
		pcbFree = pcbFree->p_next;
	}

	pcbFree->p_next = pcbFree_h;
}

