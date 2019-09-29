#include "shell.h"

uint8_t main(int argc, char **argv)
{
    //Load config

    // Run command loop
    shloop();

    //Cleanup

    return EXIT_SUCCESS;
}
