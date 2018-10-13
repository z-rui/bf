#ifndef BF_H
#define BF_H

#define DSIZE 65536   /* DATA MEM: 64KB */
#define ISIZE 262144  /* INST MEM: 256K inst */

#include <stdio.h>

typedef unsigned short addr_t;
typedef unsigned char data_t;
typedef struct {
	unsigned char opcode;
	int arg;
	int shamt;
} inst_t;

extern int bf_load(inst_t imem[], FILE *f);
extern void bf_exec(inst_t imem[], data_t dmem[]);
extern void bf_dump(inst_t imem[]);
extern void bf_to_c(inst_t imem[], FILE *f);

#endif /* BF_H */
