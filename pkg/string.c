#include "prelude.h"
#include "string.h"

void String_Teardown(void *arg) {
    String *s = arg;
    Bytes_Teardown(&s->bytes);
}

Bool String_None(String *s) {
    return Bytes_None(&s->bytes);
}

void String_ViewBytes(StringView *v, Bytes *b, Error *err) {
    (void)v;
    (void)b;
    (void)err;
    TODO();
}
