#include <stdio.h>
#include <netdb.h>
#include "rat.h"

#define usage printf("Usage: %s <domain> <file>\n", *argv)

int main(int argc, char **argv) {

	if (argc != 3) {

		usage;

		return 1;

	}

	FILE *fp = fopen(argv[2], "r");

	if (fp == NULL) {

		perror("fopen");

		return 2;
	}

	if (!readable(fp)) {

		perror("Not readable");

		return 3;

	}


	struct line *r;
	unsigned sz;

	do {

		r = nextline(fp, argv[1]);
		sz = wincols();

		if (r->buf) {

			printf("\r• %-*s", sz - 2, r->buf);

			fflush(stdout);

			if (gethostbyname(r->buf))
				printf("\r✓ %-*s\n", sz - 2, r->buf);

			free((void *)r->buf);

		}

	} while (!r->eof);

	printf("\r%-*s\n", sz, "+ Scan finished.");

	return 0;
}
