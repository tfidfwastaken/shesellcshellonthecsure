#include <stdint.h>
#include <stdlib.h>
#define SH_TOK_BUFSIZE 64
#define SH_TOK_DELIMS " \t\r\n\a"

void shloop(void);
uint8_t sh_execute(char **args);

// parsing shell input
char *sh_read_line();
char **sh_split_line(char *line);

// creating processes
uint8_t sh_launch(char **args);

// Shell builtin definitions
uint8_t sh_cd(char **args);
uint8_t sh_help(char **args);
uint8_t sh_exit(char **args);

// Array of shell builtin names and functions pointers
extern char *builtin_str[];
extern uint8_t (*builtin_funcs[])(char **);

// to get number of builtins
size_t sh_num_builtins();
