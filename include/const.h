#ifndef CONST_H
#define CONST_H

/* Kaya specific constants */
/* Maxi number of overall (eg, system, daemons, user) concurrent processes */
#define MAXPROC 20

#define UPROCMAX 3  /* number of usermode processes (not including master proc
											 and system daemons */

/* general purpose constants */
#define EXTERN extern
#define HIDDEN static
#define FALSE 0
#define TRUE 1

#define NULL ((void *)0)

#endif
