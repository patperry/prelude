#include <stdarg.h>
#include "prelude.h"
#include "prelude/string.h"

void Error_New(Error *e, String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Error_NewFromArgList(e, fmt, ap);
    va_end(ap);
}

void Error_NewFromArgList(Error *e, String *fmt, va_list ap) {
    Open();
    StringBuilder b = StringBuilder_Init;
    Defer(StringBuilder_Drop, &b);
    StringBuilder_WriteFormatArgList(&b, fmt, ap);
    StringBuilder_ToString(&b, &e->string);
    Close();
}

void Error_Drop(void *arg) {
    Error *e = arg;
    String_Drop(&e->string);
}

Bool Error_None(Error *e) {
    return String_None(&e->string);
}

Bool Error_Some(Error *e) {
    return String_Some(&e->string);
}
