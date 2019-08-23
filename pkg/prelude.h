#ifndef PRELUDE_H
#define PRELUDE_H

#include <stddef.h>
#include <limits.h>

/* Primitive data types */

typedef enum {
    None = -1,
    False = 0,
    True = 1
} Bool;

typedef int Int;
#define Int_None INT_MIN
#define Int_Min (INT_MIN + 1)
#define Int_Max INT_MAX

typedef struct Bytes {
    unsigned char *ptr;
    Int len;
} Bytes;

Bytes *B(const char *str);

void Bytes_Teardown(void *arg);
Bool Bytes_None(Bytes *b);

typedef struct String {
    Bytes bytes;
} String;

String *S(const char *str);

void String_Teardown(void *arg);
Bool String_None(String *s);

typedef struct Error {
    String string;
} Error;

void Error_Setup(Error *e, String fmt, ...);
void Error_Teardown(void *arg);
Bool Error_None(Error *e);

/* Runtime */

void Initialize(void);
void Finalize(void);

void TODO(void);

/* Strings */

#endif /* PRELUDE_H */
