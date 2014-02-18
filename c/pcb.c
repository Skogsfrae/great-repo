#include "../h/const.h"
#include "../h/pcb.h"

pcb_t *pcbFree, *pcbFree_h, *pcbFree_t;

void initPcbs()
{
	int i;
	static pcb_t *procp[MAXPROC];

	pcbFree_h = procp[0];

	for(i=0; i < MAXPROC; i++)
	{
		pcbFree = procp[i];
		pcbFree->p_next = procp[i+1];
		//pcbFree->p_prnt = pcbFree->p_child = pcbFree->p_sib = NULL;
		//pcbFree->p_s = 0;
		//pcbFree->p_semAdd = NULL;           commentati perche' verranno inizializzati a NULL/0 dalla funzione allocPcb
		pcbFree = pcbFree->p_next;
	}

	pcbFree_t = pcbFree;
	pcbFree->p_next = pcbFree_h;
}

pcb_t *allocPcb()
{
	pcb_t *pcbTmp;

	if(pcbFree_h == NULL)
		return NULL;

	pcbTmp = pcbFree_h;

	pcbFree_h->p_next = (pcbFree_h->p_next)->p_next;
	pcbFree_t->p_next = pcbFree_h;

	pcbTmp->p_prnt = pcbTmp->p_child = pcbTmp->p_sib = NULL;
	pcbTmp->p_s = 0;
	pcbTmp->p_semAdd = NULL;

	return pcbTmp;
}

void freePcb(pcb_t *p)
{
	p->next = pcbFree_h;
	pcbFree_t->p_next = p;

	pcbFree_t = p;
}

