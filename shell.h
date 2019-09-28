#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1
#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIMS "\t\r\n\a"

void shloop(void);
char *sh_read_line();
char **sh_split_line(char *line);
