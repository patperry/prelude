#include "prelude.h"

void Bytes_Teardown(void *arg) {
    Bytes *b = arg;
    (void)b;
    TODO();
}

Bool Bytes_None(Bytes *b) {
    return b->ptr == NULL;
}
