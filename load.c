#include <stdio.h>
#include "bf.h"
#include "bf_inst.h"

static inst_t *mergeshift(inst_t imem[], inst_t *ip, int opcode)
{
	if (ip > imem && ip[-1].opcode == ADD && ip[-1].arg == 0) {
		ip[-1].opcode = opcode;
		return ip - 1;
	}
	ip->opcode = opcode;
	ip->arg = ip->shamt = 0;
	return ip;
}

static inst_t *incdec(inst_t imem[], inst_t *ip, int neg)
{
	/* merge with ADD or LOAD */
	switch (ip > imem ? ip[-1].opcode : 0) {
	case ADD: case LOAD:
		--ip;
		break;
	default:
		ip->opcode = ADD;
		ip->arg = ip->shamt = 0;
		break;
	}
	neg ? --ip->arg : ++ip->arg;
	return ip;
}

static inst_t *loop_optimization(inst_t imem[], inst_t *begin, inst_t *end)
{
	inst_t *ip, *jp;
	int shamt = 0, self_add = 0;

	/* first pass - determine optimization */
	for (ip = begin + 1; ip < end; ip++) {
		if (ip->opcode != ADD)
			return end;
		shamt += ip->shamt;
		if (shamt == 0)
			self_add += ip->arg;
	}
	if (shamt != 0 || self_add != -1)
		return end;

	/* second pass - emit instructions */
	jp = begin;
	for (ip = begin + 1; ip < end; ip++) {
		shamt += ip->shamt;
		if (shamt != 0) {
			jp->opcode = OMA;
			jp->arg = ip->arg;
			jp->shamt = shamt;
			jp++;
		}
	}
	return mergeshift(imem, jp, LOAD);
}

int bf_load(inst_t imem[], FILE *f)
{
	static inst_t *jstack[ISIZE];
	inst_t **jsp;
	int ch;
	inst_t *ip;

	ip = imem;
	jsp = jstack;
	while ((ch = fgetc(f)) != EOF) {
		switch (ch) {
		case '-': case '+':
			ip = incdec(imem, ip, ch == '-');
			break;
		case '<': case '>':
			ip = mergeshift(imem, ip, ADD);
			(ch == '<') ? --ip->shamt : ++ip->shamt;
			break;
		case '[': case ']':
			ip->opcode = JMP;
			if (ch == '[') {
				ip->arg = 0;
				*jsp++ = ip;
			} else {
				if (jsp == jstack) {
					fprintf(stderr, "!too many ]'s\n");
					return -1;
				}
				--jsp;
				ip->shamt = (*jsp)->shamt = ip - *jsp;
				ip->arg = 1;
				ip = loop_optimization(imem, *jsp, ip);
			}
			break;
		case ',': case '.':
			ip = mergeshift(imem, ip, IO);
			ip->arg = (ch == '.');
			break;
		default:
			continue;
		}
		if (++ip == imem + ISIZE - 1) {
			fprintf(stderr, "!imem overflow\n");
			return -2;
		}
	}
	if (jsp != jstack) {
		fprintf(stderr, "!too many ['s\n");
		return -1;
	}
	ip->opcode = HALT;
	return 0;
}
