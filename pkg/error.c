#include "prelude.h"

void Error_Setup(Error *e, String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Error_SetupWithArgList(e, fmt, ap);
    va_end(ap);
}

void Error_SetupWithArgList(Error *e, String *fmt, va_list ap) {
    String_SetupWithArgList(&e->string, fmt, ap);
}

void Error_Teardown(void *arg) {
    Error *e = arg;
    String_Teardown(&e->string);
}

Bool Error_None(Error *e) {
    return String_None(&e->string);
}

Bool Error_Some(Error *e) {
    return String_Some(&e->string);
}
