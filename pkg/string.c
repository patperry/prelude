#include <stdio.h>
#include <string.h>
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

void String_Split(String *s, String *sep, StringView *head, StringView *tail) {
    BytesView xhead, xtail;
    Bytes_Split(&s->bytes, &sep->bytes, &xhead, &xtail);
    head->string.bytes = xhead.bytes;
    tail->string.bytes = xtail.bytes;
}

void StringBuilder_Setup(StringBuilder *b) {
    BytesBuilder_Setup(&b->bytes);
}

void StringBuilder_Teardown(void *arg) {
    StringBuilder *b = arg;
    BytesBuilder_Teardown(&b->bytes);
}

void StringBuilder_WriteChar(StringBuilder *b, Char c) {
    Assert(0 <= c && c <= 0x7f); // TODO: handle non-ASCII
    BytesBuilder_WriteByte(&b->bytes, (Byte)c);
}

void StringBuilder_WriteInt(StringBuilder *b, Int x) {
    if (x == Int_None) {
        StringBuilder_WriteString(b, S("(none)"));
        return;
    }
    char buf[32];
    Int n = (Int)sprintf(buf, "%d", x);
    Bytes fmt = {.ptr = (Byte *)buf, .len = n};
    String str = {fmt};
    StringBuilder_WriteString(b, &str);
}

void StringBuilder_WriteString(StringBuilder *b, String *s) {
    BytesBuilder_WriteBytes(&b->bytes, &s->bytes);
}

void StringBuilder_WriteFormat(StringBuilder *b, String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    StringBuilder_WriteFormatArgList(b, fmt, ap);
    va_end(ap);
}

void StringBuilder_WriteFormatArgList(StringBuilder *b, String *fmt,
                                      va_list ap) {
    Int i;
    String *s;
    String *sep = S("%");

    StringView left = {*fmt};

    while (String_Some(&left.string)) {
        StringView head, tail;
        String_Split(&left.string, sep, &head, &tail);

        StringBuilder_WriteString(b, &head.string);

        if (String_Some(&tail.string)) {
            // TODO: handle unicode
            Byte fmtc = tail.string.bytes.ptr[0];
            switch (fmtc) {
            case '%':
                StringBuilder_WriteChar(b, '%');
                break;
            case 'd':
                i = va_arg(ap, Int);
                StringBuilder_WriteInt(b, i);
                break;
            case 's':
                s = va_arg(ap, String *);
                StringBuilder_WriteString(b, s);
                break;
            default:
                Panic(S("unrecognized format %c"), fmtc);
            }

            // TODO: this is a hack
            left.string.bytes.ptr = tail.string.bytes.ptr + 1;
            left.string.bytes.len = tail.string.bytes.len - 1;
        } else {
            left.string = (String){};
        }
    }
}

void String_SetupWithBuilder(String *s, StringBuilder *b) {
    Bytes_SetupWithBuilder(&s->bytes, &b->bytes);
}
