#include <stdio.h>
#include <string.h>
#include "prelude/string.h"

void String_FromCopy(String *s, String *other) {
    Bytes_FromCopy(&s->bytes, &other->bytes);
}

void String_Drop(void *arg) {
    String *s = arg;
    Bytes_Drop(&s->bytes);
}

Bool String_None(String *s) {
    return Bytes_None(&s->bytes);
}

Bool String_Some(String *s) {
    return Bytes_Some(&s->bytes);
}

Bool String_Eq(String *s1, String *s2) {
    return Bytes_Eq(&s1->bytes, &s2->bytes);
}

void String_ViewBytes(StringView *v, Bytes *b, Error *err) {
    // TODO
    (void)err;
    v->string.bytes = *b;
}

Bool String_UnCons(String *s, Char *head, StringView *tail) {
    if (String_None(s)) {
        *head = Char_None;
        tail->string = *s;
        return False;
    } else {
        *head = s->bytes.ptr[0];
        Assert(*head <= 0x7F); // TODO: Unicode
        tail->string.bytes.ptr = s->bytes.ptr + 1;
        tail->string.bytes.len = s->bytes.len - 1;
        return True;
    }
}

void String_Split(String *s, String *sep, StringView *head, StringView *tail) {
    BytesView xhead, xtail;
    Bytes_Split(&s->bytes, &sep->bytes, &xhead, &xtail);
    head->string.bytes = xhead.bytes;
    tail->string.bytes = xtail.bytes;
}

void String_Gen(String *s, Int n) {
    (void)n;
    String_FromCopy(s, S("hello string")); // TODO: better
}

void StringBuilder_Drop(void *arg) {
    StringBuilder *b = arg;
    BytesBuilder_Drop(&b->bytes);
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
    Char c, fmtc;
    Int i;
    String *s, *sep = S("%");
    StringView pre, post, left;

    left.string = *fmt;

    while (String_Some(&left.string)) {
        String_Split(&left.string, sep, &pre, &post);
        StringBuilder_WriteString(b, &pre.string);

        if (String_UnCons(&post.string, &fmtc, &left)) {
            switch (fmtc) {
            case '%':
                StringBuilder_WriteChar(b, '%');
                break;
            case 'c':
                c = va_arg(ap, Char);
                StringBuilder_WriteChar(b, c);
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
        } else {
            left.string = String_Init;
        }
    }
}

void StringBuilder_ToString(StringBuilder *b, String *s) {
    BytesBuilder_ToBytes(&b->bytes, &s->bytes);
}
