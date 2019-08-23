#include "prelude.h"

void Error_Setup(Error *e, String fmt, ...) {
    (void)e;
    (void)fmt;
    TODO();
}

void Error_Teardown(void *arg) {
    Error *e = arg;
    String_Teardown(&e->string);
}

Bool Error_None(Error *e) {
    return String_None(&e->string);
}
