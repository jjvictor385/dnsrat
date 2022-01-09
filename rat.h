#ifndef RAT_H

#define RAT_H
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

struct line {
	char const* buf;
	_Bool eof;
};

unsigned wincols() {

	struct winsize w;

	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	return w.ws_col;
}

_Bool readable(FILE *fp) {

	char j;

	fread(&j, 1, 1, fp);

	if (ferror(fp)) {

		return 0;

	}

	fseek(fp, 0, SEEK_SET);

	return 1;

}

struct line *nextline(FILE *fp, char const* end) {

	size_t i = 0, ln = strlen(end), j;
	char ch, *buf = NULL /* Hold the complete string */;

	static struct line r = {NULL, 0};

	while (1) {

		fread(&ch, 1, 1, fp);

		if (feof(fp) || ch == 0xa) {

			if (buf)
				strcat(buf, end);

			r.buf = buf;
			r.eof = feof(fp);

			break;

		}

		char tmp[i + 1];

		for (j = 0; j < i; j++)
			tmp[j] = buf[j];

		tmp[j] = ch;

		if (buf)
			free(buf);

		buf = malloc(ln + ++i + 2);

		for (j = 0; j < i; j++)
			buf[j] = tmp[j];

		buf[j] = 0;

	}

	return &r;

}

#endif