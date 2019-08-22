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

#define Bytes_None NULL
Bytes *B(const char *str);

typedef struct String {
    Bytes bytes;
} String;

#define String_None Bytes_None
String *S(const char *str);

typedef struct Error {
    String string;
} Error;

#define Error_None String_None

/* Runtime */

void Initialize(void);
void Finalize(void);

/* Strings */

#endif /* PRELUDE_H */
