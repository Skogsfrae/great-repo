#ifndef PCB_H
#define PCB_H

/* process control block type */
typedef struct pcb_t {
	/* process queue fields */
	struct pcb_t *p_next, /* pointer to next entry */
							 /* process tree fields */
							 *p_prnt, /* pointer to parent */
							 * p_child, /* pointer to 1st child */
							 * p_sib; /* pointer to sibling*/
	state_t p_s; /* processor state */
	int *p_semAdd; /* pointer to sema4 on which process blocked */
	/* plus other entries to be added later */
} pcb_t;

/* PCB handling functions */

/* List view functions */

EXTERN void freePcb(pcb_t *p);
EXTERN pcb_t *allocPcb(void);
EXTERN void initPcbs(void);
EXTERN pcb_t *mkEmptyProcQ(void);
EXTERN int emptyProcQ(pcb_t *tp);
EXTERN void insertProcQ(pcb_t **tp, pcb_t *p);
EXTERN pcb_t *removeProcQ(pcb_t **tp);
EXTERN pcb_t *outProcQ(pcb_t **tp, pcb_t *p);
EXTERN pcb_t *headProcQ(pcb_t *tp);

/* Tree view functions */

EXTERN void initASL(void);
EXTERN int emptyChild(pcb_t *p);
EXTERN void insertChild(pcb_t *prnt, pcb_t *p);
EXTERN pcb_t *removeChild(pcb_t *p);
EXTERN pcb_t *outChild(pcb_t *p);

#endif
