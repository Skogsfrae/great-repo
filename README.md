great-repo
==========

Kaya project

Documentation of Kaya OS v1.2
by Dario Bonesi, Leopoldo Lunghi, Mirko Mucaria, Tonino Petrulli


1. Introduction

Phase1 of Kaya OS aims to implement the data structures and functions to manage them.


2. Data structures

Main data structures are process blocks and semaphore descriptors.
We decided to implement pcb_t (the process block structure) adding a pointer to the previous entry in order to more easily operate on pcb_t lists (which, doing so, became double linked) such as queues.
Semaphore descriptor structur was kept as in the documentation of Kaya OS.


3. Implementation decisions

Exept for pcb_t struct we decided to keep everything else as specified in Kaya OS documentation.


4. Code

We organized the code as suggested on the last paragaph of Kaya OS documentation.
