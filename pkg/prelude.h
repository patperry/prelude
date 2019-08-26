#ifndef PRELUDE_H
#define PRELUDE_H

#include <stdarg.h>
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

typedef unsigned char Byte;

typedef struct Bytes {
    Byte *ptr;
    Int len;
} Bytes;

Bytes *B(const char *str);

void Bytes_Teardown(void *arg);

Bool Bytes_Some(Bytes *b);
Bool Bytes_None(Bytes *b);

typedef Int Char;

typedef struct String {
    Bytes bytes;
} String;

String *S(const char *str);

void String_Setup(String *s, String *fmt, ...);
void String_SetupWithArgList(String *s, String *fmt, va_list ap);
void String_Teardown(void *arg);

Bool String_Some(String *s);
Bool String_None(String *s);


typedef struct Error {
    String string;
} Error;

void Error_Setup(Error *e, String *fmt, ...);
void Error_SetupWithArgList(Error *e, String *fmt, va_list ap);
void Error_Teardown(void *arg);

Bool Error_Some(Error *e);
Bool Error_None(Error *e);


/* Runtime */

void Initialize(void);
void Finalize(void);

void Open(void);
void Close(void);

void Panic(String *fmt, ...) __attribute__((noreturn));
void Defer(void (*func)(void *arg), void *arg);
void Trap(void (*func)(void *arg), void *arg);


/* Memory allocation */

void *Alloc(Int len);
void *Realloc(void *ptr, Int from, Int to);
void Free(void *ptr, Int len);

#define SizeOf(x) (Int)sizeof(x)

typedef struct Memory {
    void *ptr;
    Int len;
} Memory;

void Memory_Setup(Memory *mem, Int len);
void Memory_Teardown(void *arg);


#define Assert(expr) \
    do { \
        if (!(expr)) { \
            Panic(S("assertion \"%s\" failed: function %s, file \"%s\", line %d"), \
                  "expr", S(__func__), S(__FILE__), __LINE__); \
        } \
    } while (0)

#define TODO() \
    Panic(S("not implemented: function %s, file \"%s\", line %d"), \
          S(__func__), S(__FILE__), __LINE__)

#endif /* PRELUDE_H */
