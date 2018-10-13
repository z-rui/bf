#include <stdio.h>
#include "bf.h"
#include "bf_inst.h"

void bf_exec(inst_t imem[], data_t dmem[])
{
	addr_t ptr = 0;
	inst_t *ip;

	for (ip = imem; ip < imem + ISIZE; ip++) {
		switch (ip->opcode) {
		case HALT:
			return;
		case ADD:
			ptr = (ptr + ip->shamt) % DSIZE;
			dmem[ptr] += ip->arg;
			break;
		case JMP:
			if (!ip->arg && !dmem[ptr])
				ip += ip->shamt;
			else if (ip->arg && dmem[ptr])
				ip -= ip->shamt;
			break;
		case IO:
			ptr = (ptr + ip->shamt) % DSIZE;
			if (ip->arg)
				putchar(dmem[ptr]);
			else
				dmem[ptr] = getchar();
			break;
		case LOAD:
			ptr = (ptr + ip->shamt) % DSIZE;
			dmem[ptr] = ip->arg;
			break;
		case OMA:
			dmem[(ptr + ip->shamt) % DSIZE] += ip->arg * dmem[ptr];
			break;
		default:
			fprintf(stderr, "!invalid instruction\n");
			break;
		}
	}
}
