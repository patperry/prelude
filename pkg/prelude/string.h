#ifndef PRELUDE_STRING_H
#define PRELUDE_STRING_H

#include "prelude.h"
#include "prelude/array.h"

typedef struct StringBuilder {
    ByteArray buf;
    String string;
} StringBuilder;

void StringBuilder_Setup(StringBuilder *b);
void StringBuilder_Teardown(void *arg);
void StringBuilder_WriteFormat(StringBuilder *b, String *fmt, ...);
void StringBuilder_WriteFormatArgList(StringBuilder *b, String *fmt,
                                      va_list ap);

void String_SetupWithBuilder(String *s, StringBuilder *b);


typedef struct StringView {
    String string;
} StringView;

void String_ViewBytes(StringView *v, Bytes *b, Error *err);

#endif /* PRELUDE_STRING_H */
