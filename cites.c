#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUF 1024

void usage(void);

void scan_cites(FILE *fp); 
int is_cite(char buffer[]);
void print_cite(int line, char buffer[]); 

static regex_t recite, remath;
int nflag;

int main(int argc, char *argv[]) {
	FILE *fp;
	int ch;

	nflag = 0;
	while ((ch = getopt(argc, argv, "n")) != -1) {
		switch (ch) {
			case 'n':
				nflag = 1;
				break;
			default:
				usage();
				return 1;
		}
	}
	argc -= optind;
	argv += optind;

	if (argc == 0) {
		scan_cites(stdin);
	} else {
		fp = fopen(*argv, "r");
		if (fp == NULL) {
			usage();
			return 1;
		}

		scan_cites(fp);
		fclose(fp);
	}

	regfree(&recite);
	regfree(&remath);
	
	return 0;
}

void scan_cites(FILE *fp) {
	int parflag = 0;  // is inside parenthesis?

	int ch;
	int len = 0;
	int line = 1;
	int cite_line = 1;

	char buffer[MAX_BUF];
	memset(buffer, '\0', MAX_BUF);

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '(') {
			parflag = 1;
			cite_line = line;
		}

		if (ch == '\n') {
			line++;
		}

		if (parflag) {
			if (len < MAX_BUF - 1) {
				buffer[len++] = (char)(ch == '\n' ? ' ' : ch);
			} else {
				parflag = 0;
			}

			if ((ch == ')')) {
				parflag = 0;
			}


			if (!parflag) {
				buffer[len] = '\0';
				if (is_cite(buffer)) {
					print_cite(cite_line, buffer);
				}
				memset(buffer, '\0', MAX_BUF);
				len = 0;
			}
		}
	}

}

int is_cite(char buffer[]) {
	/* Return values:
	   	 1: buffer is a cite
		 0: buffer does not look like a cite
		-1: error
	*/

	int ret = 0;
	static int compiled = 0;

	if (!compiled) {
		if ((regcomp(&recite, "[A-Za-z].+[0-9]{4}", REG_EXTENDED) != 0) ||
			(regcomp(&remath, "[=<>+]", REG_EXTENDED) != 0)) {
			fprintf(stderr, "Can't compile regexp\n.");
			return -1;
		}
		compiled = 1;
	}

	if ((!regexec(&recite, buffer, 0, NULL, 0)) && 
		(regexec(&remath, buffer, 0, NULL, 0))) {
		ret = 1;
	}

	return ret;
}

void print_cite(int line, char buffer[]) {
	if (nflag) {
		printf("%4d  %s\n", line, buffer);
	} else {
		printf("%s\n", buffer);
	}
}

void usage(void) {
	puts("usage: cites [-n] [file]");
	puts("  file  Paper in plain text. Read from stdin if absent");
	puts("  -n    Print line where cite starts");
	puts("\nExample: ");
	puts("pandoc -t Paper.docx | cites -n");
	return;
}
