#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include "shell.h"

void shloop(void)
{
    char *line;
    char **args;
    uint8_t status;

    do {
        printf("> ");
        line = sh_read_line();
        args = sh_split_line(line);
        status = sh_exec(args);

        free(line);
        free(args);
    } while(status);
}

char *sh_read_line()
{
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **sh_split_line(char *line)
{
    size_t bufsize = SH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(sizeof(char) * bufsize);
    char *token;

    if(!tokens) {
        fprintf(stderr, "she sell: allocation error!");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SH_TOK_DELIMS);
    while(token != NULL) {
        token[position] = token;
        position++;

        if(position >= bufsize) {
            bufsize += SH_TOK_DELIMS;
            tokens = realloc(tokens, sizeof(char) * bufsize);

            if(!tokens) {
                fprintf(stderr, "she sell: allocation error!");
                exit(EXIT_FAILURE);
            }
        }
    }

    token = strtok(NULL, SH_TOK_DELIMS);
    return tokens;
}

uint8_t sh_launch(char **args)
{
    pid_t pid, wpid;
    uint8_t status;

    pid = fork();
    if(pid == 0) {
        if(execvp(args[0], args) == -1) {
            perror("shesell");
        }
    } else if(pid < 0) {
        perror("shesell");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

uint8_t main(int argc, char **argv)
{
    //Load config

    // Run command loop
    shloop();

    //Cleanup

    return EXIT_SUCCESS;
}
