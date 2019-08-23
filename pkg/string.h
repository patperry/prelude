#ifndef STRING_H
#define STRING_H

typedef struct StringView {
    String string;
} StringView;

void String_ViewBytes(StringView *v, Bytes *b, Error *err);

#endif /* STRING_H */
