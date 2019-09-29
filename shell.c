#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "shell.h"

// Initialising builtin names and functions
char *builtin_str[] = {
    "cd",
    "help",
    "exit"
};

uint8_t (*builtin_funcs[])(char **) = {
    &sh_cd,
    &sh_help,
    &sh_exit,
};


void shloop(void)
{
    char *line;
    char **args;
    uint8_t status;

    do {
        printf("> ");
        line = sh_read_line();
        args = sh_split_line(line);
        status = sh_execute(args);

        free(line);
        free(args);
    } while(status);
}

char *sh_read_line()
{
    char *line = NULL;
    size_t bufsize = 0;
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
        tokens[position] = token;
        position++;

        if(position >= bufsize) {
            bufsize += SH_TOK_BUFSIZE;
            tokens = realloc(tokens, sizeof(char *) * bufsize);

            if(!tokens) {
                fprintf(stderr, "she sell: allocation error!");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SH_TOK_DELIMS);
    }

    tokens[position] = NULL;
    return tokens;
}

uint8_t sh_launch(char **args)
{
    pid_t pid, wpid;
    int32_t status;

    pid = fork();
    if(pid == 0) {
        if(execvp(args[0], args) == -1) {
            perror("she sell");
        }
        exit(EXIT_FAILURE);
    } else if(pid < 0) {
        perror("she sell");
    } else {
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while(!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

size_t sh_num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

uint8_t sh_cd(char **args)
{
    if(args[1] == NULL) {
        fprintf(stderr, "she sell: expected argument to \"cd\"\n");
    } else {
        if(chdir(args[1]) != 0) {
            perror("she sell");
        }
    }
    
    return 1;
}
    
uint8_t sh_help(char **args)
{
    printf("She Sells C Shells on the C, Sure!\n");
    printf("Made by Atharva Raykar, based on Stephen Brennan's LSH\n");
    printf("Built-ins:\n");

    for(size_t i = 0; i < sh_num_builtins(); ++i) {
        printf("  %s\n", builtin_str[i]);
    }

    return 1;
}

uint8_t sh_exit(char **args)
{
    return 0;
}

uint8_t sh_execute(char **args)
{
    if(args[0] == NULL) {
        // empty command
        return 1;
    }

    for(size_t i = 0; i < sh_num_builtins(); ++i) {
        if(strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_funcs[i])(args);
        }
    }

    return sh_launch(args);
}
