#include <stdio.h>
#include "bf.h"

#define EXEC 0
#define DUMP 1
#define TO_C 2

int main(int argc, char *argv[])
{
	static inst_t imem[ISIZE];
	static data_t dmem[DSIZE];
	FILE *f;
	int rc;
	int mode = EXEC;
	char *progname;

	(void) argc;
	progname = *argv++;
	if (*argv && (*argv)[0] == '-') {
		switch ((*argv)[1]) {
		case 'd':
			mode = DUMP;
			break;
		case 'c':
			mode = TO_C;
			break;
		}
		++argv;
	}
	if (*argv == NULL) {
		fprintf(stderr, "Usage: %s [-cd] bf_src\n", progname);
		return 1;
	}
	if ((f = fopen(*argv, "r")) == NULL) {
		perror("fopen");
		return 1;
	}
	rc = bf_load(imem, f);
	fclose(f);
	if (rc != 0)
		return 1;
	switch (mode) {
	case EXEC:
		bf_exec(imem, dmem);
		break;
	case DUMP:
		bf_dump(imem);
		break;
	case TO_C:
		bf_to_c(imem, stdout);
		break;
	}
	return 0;
}
