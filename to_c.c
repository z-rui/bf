#include <stdio.h>
#include "bf.h"
#include "bf_inst.h"

void bf_to_c(inst_t imem[], FILE *f)
{
	inst_t *ip;

	fprintf(f, "\
#include <stdio.h>\n\
int main()\n\
{\n\
	static unsigned char mem[%u];\n\
	unsigned short ptr = 0;\n\
", DSIZE);

	for (ip = imem; ip < imem + ISIZE && ip->opcode != HALT; ip++) {
		switch (ip->opcode) {
		case ADD:
			printf("\tmem[ptr+=%d]+=%d;\n", ip->shamt, ip->arg);
			break;
		case JMP:
			if (ip->arg == 0)
				printf("\twhile(mem[ptr]) {\n");
			else
				printf("\t}\n");
			break;
		case IO:
			if (ip->arg == 0)
				printf("\tmem[ptr+=%d]=getchar();\n", ip->shamt);
			else
				printf("\tputchar(mem[ptr+=%d]);\n", ip->shamt);
			break;
		case LOAD:
			printf("\tmem[ptr+=%d]=%d;\n", ip->shamt, ip->arg);
			break;
		case OMA:
			printf("\tmem[ptr+%d]+=mem[ptr]*%d;\n", ip->shamt, ip->arg);
			break;
		}
	}
	printf("\treturn 0;\n}\n");
}
