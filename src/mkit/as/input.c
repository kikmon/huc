#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include "defs.h"
#include "externs.h"
#include "protos.h"

#define INITIAL_PATH_SIZE 64
#define INCREMENT_BASE 16
#define INCREMENT_BASE_MASK 15

int    infile_error;
int    infile_num;
struct t_input_info input_file[8];
static char    *incpath			= NULL;
static int	   *str_offset		= NULL;
static int	   remaining		= 0;
static int     incpathSize		= 0;
static int     str_offsetCount	= 0;
static int     incpathCount		= 0;

/* ----
 * void cleanup_path()
 * ----
 * clean up allocated paths
 */
void
cleanup_path(void)
{
	if(incpath)
		free(incpath);
		
	if(str_offset)
		free(str_offset);
} 

/* ----
 * int add_path(char*, int)
 * ----
 * add a path to includes
 */
int
add_path(char* path, int l)
{
	/* Expand str_offset array if needed */
	if(incpathCount >= str_offsetCount)
	{
		str_offsetCount += INCREMENT_BASE;
		str_offset = (int*)realloc(str_offset, str_offsetCount * sizeof(int));
		if(str_offset == NULL)
			return 0;
	}

	/* Initialize string offset */
	str_offset[incpathCount] = incpathSize - remaining;

	/* Realloc string buffer if needed */
	if(remaining < l)
	{
		remaining  = incpathSize;
		/* evil trick, get the greater multiple of INCREMENT_BASE closer 
		   to (size + l). Note : this only works for INCREMENT_BASE = 2^n*/
		incpathSize = ((incpathSize + l) + INCREMENT_BASE) & ~INCREMENT_BASE_MASK;
		remaining  = incpathSize - remaining;
		incpath = (char*)realloc(incpath, incpathSize);
		if(incpath == NULL)
			return 0;
	}
		
	remaining -= l;

	/* Copy path */
	strncpy(incpath + str_offset[incpathCount], path, l);
	incpath[str_offset[incpathCount] + l - 1] = '\0';
	
	++incpathCount;
	
	return 1;
}

#ifdef WIN32
#define ENV_PATH_SEPARATOR ';'
#else
#define ENV_PATH_SEPARATOR ':'
#endif

/* ----
 * int init_path()
 * ----
 * init the include path
 */

int
init_path(void)
{
	char *p,*pl;
	int	ret, l;

	/* Get env variable holding PCE path*/
	p = getenv(machine->include_env);
	printf("%s = \"%s\"\n\n", machine->include_env, p);

	if (p == NULL)
		return 2;

	l  = 0;
	pl = p;
	while(pl != NULL)
	{
		
		/* Jump to next separator */
		pl = strchr(p, ENV_PATH_SEPARATOR);

		/* Compute new substring size */
		if(pl == NULL)
			l = strlen(p) + 1;
		else
			l = pl - p + 1;
			
		/* Might be empty, jump to next char */
		if(l <= 1)
		{
			++p;
			continue;
		}

		/* Add path */
		ret = add_path(p, l);
		if(!ret)
			return 0;

		/* Eat remaining separators */
		while (*p == ENV_PATH_SEPARATOR) ++p;
		
		p += l;
	}
	
	return 1;
}


/* ----
 * readline()
 * ----
 * read and format an input line.
 */

int
readline(void)
{
	char *ptr, *arg, num[8];
	int j, n;
	int i;		/* pointer into prlnbuf */
	int c;		/* current character		*/
	int temp;	/* temp used for line number conversion */

start:
	memset(prlnbuf, ' ', SFIELD - 1);
	prlnbuf[SFIELD - 1] = '\t';

	/* if 'expand_macro' is set get a line from macro buffer instead */
	if (expand_macro) {
		if (mlptr == NULL) {
			while (mlptr == NULL) {
				midx--;
				mlptr = mstack[midx];
				mcounter = mcntstack[midx];
				if (midx == 0) {
					mlptr = NULL;
					expand_macro = 0;
					break;
				}
			}
		}

		/* expand line */
		if (mlptr) {
			i = SFIELD;
			ptr = mlptr->data;
			for (;;) {
				c = *ptr++;
				if (c == '\0')
					break;
				if (c != '\\')
					prlnbuf[i++] = c;
				else {
					c = *ptr++;
					prlnbuf[i] = '\0';

					/* \@ */
					if (c == '@') {
						n = 5;
						sprintf(num, "%05i", mcounter);
						arg = num;
					}

					/* \# */
					else if (c == '#') {
						for (j = 9; j > 0; j--)
							if (strlen(marg[midx][j - 1]))
								break;
						n = 1;
						sprintf(num, "%i", j);
						arg = num;
					}

					/* \?1 - \?9 */
					else if (c == '?') {
						c = *ptr++;
						if (c >= '1' && c <= '9') {
							n = 1;
							sprintf(num, "%i", macro_getargtype(marg[midx][c - '1']));
							arg = num;
						}
						else {
							error("Invalid macro argument index!");
							return (-1);
						}
					}

					/* \1 - \9 */
					else if (c >= '1' && c <= '9') {
						j = c - '1';
						n = strlen(marg[midx][j]);
						arg = marg[midx][j];
					}

					/* unknown macro special command */
					else {
						error("Invalid macro argument index!");
						return (-1);
					}

					/* check for line overflow */
					if ((i + n) >= LAST_CH_POS - 1) {
						error("Invalid line length!");
						return (-1);
					}

					/* copy macro string */
					strncpy(&prlnbuf[i], arg, n);
					i += n;
				}
				if (i >= LAST_CH_POS - 1)
					i = LAST_CH_POS - 1;
			}
			prlnbuf[i] = '\0';
			mlptr = mlptr->next;
			return (0);
		}
	}

	if (list_level) {
		/* put source line number into prlnbuf */
		i = 4;
		temp = ++slnum;
		while (temp != 0) {
			prlnbuf[i--] = temp % 10 + '0';
			temp /= 10;
		}
	}

	/* get a line */
	i = SFIELD;
	c = getc(in_fp);
	if (c == EOF) {
		if (close_input()) {
			if (stop_pass != 0 || kickc_incl == 0) {
				return (-1);
			} else {
				kickc_incl = 0;
				if (open_input("kickc-final.asm") == -1) {
					fatal_error("Cannot open \"kickc-final.asm\" file!");
					return (-1);
				}
			}
		}
		goto start;
	}
	for (;;) {
		/* check for the end of line */
		if (c == '\r') {
			c = getc(in_fp);
			if (c == '\n' || c == EOF)
				break;
			ungetc(c, in_fp);
			break;
		}
		if (c == '\n' || c == EOF)
			break;

		/* store char in the line buffer */
		prlnbuf[i] = c;
		i += (i < LAST_CH_POS) ? 1 : 0;

		/* get next char */
		c = getc(in_fp);
	}
	prlnbuf[i] = '\0';

	/* reset these at the beginning of the new line */
	preproc_sfield = SFIELD;
	preproc_modidx = 0;

	/* pre-process the input to change C-style comments into ASM ';' comments */
	if (asm_opt[OPT_CCOMMENT])
	{
		int i = SFIELD;
		int c = 0;

		/* repeat this loop until we know how the line ends */
		do {
			/* if we're in a block comment, look for the end of the block */
			if (preproc_inblock != 0) {
				for (; prlnbuf[i] != '\0'; ++i) {
					if (prlnbuf[i] == '*' && prlnbuf[i+1] == '/') {
						preproc_inblock = 0;
						if (preproc_modidx != 0 && c == 0) {
							prlnbuf[preproc_modidx] = '/';
							preproc_modidx = 0;
						}
						i = i + 2;
						if (preproc_modidx == 0) {
							preproc_sfield = i;
						}
						break;
					}
				}
			}

			/* if we're not in a block comment, look for a new comment */
			for (; prlnbuf[i] != '\0'; ++i) {
				if (prlnbuf[i] == '/') {
					if (prlnbuf[i+1] == '/') {
						if (preproc_modidx == 0) {
							preproc_modidx = i;
							prlnbuf[i] = ';';
						}
						break;
					}
					else
					if (prlnbuf[i+1] == '*') {
						preproc_inblock = 1;
						if (preproc_modidx == 0) {
							preproc_modidx = i;
							prlnbuf[i] = ';';
						}
						i = i + 2;
						break;
					}
				}
				/* remember if we see text that needs to be assembled */
				if (!isspace(prlnbuf[i])) { c = 1; }
			}

			/* repeat if we're in a block comment, and not at the EOL */
		} while (preproc_inblock != 0 && prlnbuf[i] != '\0');

		/* if we've been in a block comment for the whole line */
		if (preproc_inblock != 0 && preproc_modidx == 0) {
			preproc_sfield = i;
		}
	}

	return (0);
}

/* ----
 * open_input()
 * ----
 * open input files - up to 7 levels.
 */

int
open_input(char *name)
{
	FILE *fp;
	char *p;
	char temp[128];
	int i;

	/* only 7 nested input files */
	if (infile_num == 7) {
		error("Too many include levels, max. 7!");
		return (1);
	}

	/* backup current input file infos */
	if (infile_num) {
		input_file[infile_num].lnum = slnum;
		input_file[infile_num].fp = in_fp;
	}

	/* get a copy of the file name */
	strcpy(temp, name);

	/* auto add the .asm file extension */
	if ((p = strrchr(temp, '.')) != NULL) {
		if (strchr(p, PATH_SEPARATOR))
			strcat(temp, ".asm");
	}
	else {
		strcat(temp, ".asm");
	}

	/* check if this file is already opened */
	if (infile_num) {
		for (i = 1; i < infile_num; i++) {
			if (!strcmp(input_file[i].name, temp)) {
				error("Repeated include file!");
				return (1);
			}
		}
	}

	/* open the file */
	if ((fp = open_file(temp, "r")) == NULL)
		return (-1);

	/* update input file infos */
	in_fp = fp;
	slnum = 0;
	infile_num++;
	input_file[infile_num].fp = fp;
	input_file[infile_num].if_level = if_level;
	strcpy(input_file[infile_num].name, temp);
	if ((pass == LAST_PASS) && (xlist) && (list_level))
		fprintf(lst_fp, "#[%i]   %s\n", infile_num, input_file[infile_num].name);

	/* ok */
	return (0);
}


/* ----
 * close_input()
 * ----
 * close an input file, return -1 if no more files in the stack.
 */

int
close_input(void)
{
	if (proc_ptr) {
		fatal_error("Incomplete .proc/.procgroup!");
		return (-1);
	}
	if (scopeptr) {
		fatal_error("Incomplete .struct!");
		return (-1);
	}
	if (in_macro) {
		fatal_error("Incomplete MACRO definition!");
		return (-1);
	}
	if (input_file[infile_num].if_level != if_level) {
		char message[128];
		sprintf(message, "Incomplete IF/ENDIF statement, beginning at line %d!", if_line[if_level-1]);
		fatal_error(message);
		return (-1);
	}
	if (infile_num <= 1)
		return (-1);

	fclose(in_fp);
	infile_num--;
	infile_error = -1;
	slnum = input_file[infile_num].lnum;
	in_fp = input_file[infile_num].fp;
	if ((pass == LAST_PASS) && (xlist) && (list_level))
		fprintf(lst_fp, "#[%i]   %s\n", infile_num, input_file[infile_num].name);

	/* ok */
	return (0);
}


/* ----
 * open_file()
 * ----
 * open a file - browse paths
 */

FILE *
open_file(char *name, char *mode)
{
	FILE 	*fileptr;
	char	testname[256];
	int	i;

	fileptr = fopen(name, mode);
	if (fileptr != NULL) return(fileptr);

	for (i = 0; i < incpathCount; ++i) {
		if (strlen(incpath+str_offset[i])) {
			strcpy(testname, incpath+str_offset[i]);
			strcat(testname, PATH_SEPARATOR_STRING);
			strcat(testname, name);
		
			fileptr = fopen(testname, mode);
			if (fileptr != NULL) break;
		}
	}

	return (fileptr);
}

