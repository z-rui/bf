#include <stdio.h>
#include "bf.h"
#include "bf_inst.h"

static const char bf_name[][8] = {
	"HALT",
	"ADD",
	"JMP",
	"IO",
	"LOAD",
	"OMA",
};

void bf_dump(inst_t imem[])
{
	inst_t *ip;

	printf("\tOPCODE\tARG\tSHAMT\n");
	for (ip = imem; ip < imem + ISIZE; ip++) {
		printf("%04x\t%s\t%d\t%d",
			(unsigned) (ip - imem),
			bf_name[ip->opcode], ip->arg, ip->shamt);
		if (ip->opcode == JMP)
			printf("\t; to %04x",
				(unsigned) (ip - imem) +
				((ip->arg) ? -ip->shamt : ip->shamt));
		putchar('\n');
		if (ip->opcode == HALT)
			break;
	}
}
