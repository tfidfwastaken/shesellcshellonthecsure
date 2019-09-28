#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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

uint8_t main(int argc, char **argv)
{
    //Load config

    // Run command loop
    shloop();

    //Cleanup

    return EXIT_SUCCESS;
}
