#include "prelude/string.h"

void String_Setup(String *s, String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    String_SetupWithArgList(s, fmt, ap);
    va_end(ap);
}

void String_SetupWithArgList(String *s, String *fmt, va_list ap) {
    StringBuilder b;
    StringBuilder_Setup(&b);
    Defer(StringBuilder_Teardown, &b);

    StringBuilder_WriteFormatArgList(&b, fmt, ap);
    String_SetupWithBuilder(s, &b);
}

void String_Teardown(void *arg) {
    String *s = arg;
    Bytes_Teardown(&s->bytes);
}

Bool String_None(String *s) {
    return Bytes_None(&s->bytes);
}

Bool String_Some(String *s) {
    return Bytes_Some(&s->bytes);
}


void String_ViewBytes(StringView *v, Bytes *b, Error *err) {
    (void)v;
    (void)b;
    (void)err;
    TODO();
}


void StringBuilder_Setup(StringBuilder *b) {
    *b = (StringBuilder){};
}

void StringBuilder_Teardown(void *arg) {
    StringBuilder *b = arg;
    ByteArray_Teardown(&b->buf);
}

void StringBuilder_WriteFormat(StringBuilder *b, String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    StringBuilder_WriteFormatArgList(b, fmt, ap);
    va_end(ap);
}

void StringBuilder_WriteFormatArgList(StringBuilder *b, String *fmt,
                                      va_list ap) {
    (void)b;
    (void)fmt;
    (void)ap;
    // TODO
}

void String_SetupWithBuilder(String *s, StringBuilder *b) {
    *s = b->string;
    *b = (StringBuilder){};
}
