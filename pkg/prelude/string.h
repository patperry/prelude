#ifndef PRELUDE_STRING_H
#define PRELUDE_STRING_H

#include "prelude.h"
#include "prelude/bytes.h"

typedef struct StringBuilder {
    BytesBuilder bytes;
} StringBuilder;
#define StringBuilder_Init (StringBuilder){BytesBuilder_Init}

void StringBuilder_Drop(void *arg);
void StringBuilder_WriteChar(StringBuilder *b, Char c);
void StringBuilder_WriteInt(StringBuilder *b, Int x);
void StringBuilder_WriteString(StringBuilder *b, String *s);
void StringBuilder_WriteFormat(StringBuilder *b, String *fmt, ...);
void StringBuilder_WriteFormatArgList(StringBuilder *b, String *fmt,
                                      va_list ap);

void StringBuilder_ToString(StringBuilder *b, String *s);


typedef struct StringView {
    String string;
} StringView;

void String_ViewBytes(StringView *v, Bytes *b, Error *err);

Bool String_UnCons(String *s, Char *head, StringView *tail);
void String_Split(String *s, String *sep, StringView *head, StringView *tail);

#endif /* PRELUDE_STRING_H */
