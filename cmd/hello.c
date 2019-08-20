#include <stdlib.h>
#include <stdio.h>
#include "prelude.h"

int main(int argc, char **argv) {
    Initialize();
    printf("hello world\n");
    Finalize();
    return EXIT_SUCCESS;

    (void)argc;
    (void)argv;
}
