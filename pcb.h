#ifndef PCB_H
#define PCB_H

#include "types.h"

/* process control block type */
typedef struct pcb_t {
	/* process queue fields */
	struct pcb_t *p_next, /* pointer to next entry */
							 *p_prev,
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

extern void freePcb(pcb_t *p);
extern pcb_t *allocPcb(void);
extern void initPcbs(void);
extern pcb_t *mkEmptyProcQ(void);
extern int emptyProcQ(pcb_t *tp);
extern void insertProcQ(pcb_t **tp, pcb_t *p);
extern pcb_t *removeProcQ(pcb_t **tp);
extern pcb_t *outProcQ(pcb_t **tp, pcb_t *p);
extern pcb_t *headProcQ(pcb_t *tp);

/* Tree view functions */

extern void initASL(void);
extern int emptyChild(pcb_t *p);
extern void insertChild(pcb_t *prnt, pcb_t *p);
extern pcb_t *removeChild(pcb_t *p);
extern pcb_t *outChild(pcb_t *p);

#endif
