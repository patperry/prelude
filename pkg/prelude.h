#ifndef PRELUDE_H
#define PRELUDE_H

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

/* Primitive data types */

typedef enum {
    None = -1,
    False = 0,
    True = 1
} Bool;

typedef int64_t Int;
#define Int_None INT64_MIN
#define Int_Min (INT64_MIN + 1)
#define Int_Max INT64_MAX

typedef double Float;

typedef unsigned char Byte;
typedef uint16_t Word16;
typedef uint32_t Word32;
typedef uint64_t Word64;

#define Byte_Max UCHAR_MAX
#define Word64_Max UINT64_MAX

#define W64 UINT64_C

typedef struct Bytes {
    Byte *ptr;
    Int len;
} Bytes;
#define Bytes_Init (Bytes){NULL, 0}

Bytes *B(const char *str);

void Bytes_FromCopy(Bytes *b, Bytes *other);
void Bytes_Drop(void *arg);

Bool Bytes_Some(Bytes *b);
Bool Bytes_None(Bytes *b);
Bool Bytes_Eq(Bytes *b1, Bytes *b2);


typedef Int Char;
#define Char_None (Char)0

typedef struct String {
    Bytes bytes;
} String;

#define String_Init (String){Bytes_Init}

String *S(const char *str);

void String_FromCopy(String *s, String *other);
void String_Drop(void *arg);

Bool String_Some(String *s);
Bool String_None(String *s);
Bool String_Eq(String *s1, String *s2);


typedef struct Error {
    String string;
} Error;

#define Error_Init (Error){String_Init}
void Error_New(Error *e, String *fmt, ...);
void Error_NewFromArgList(Error *e, String *fmt, va_list ap);

void Error_Drop(void *arg);

Bool Error_Some(Error *e);
Bool Error_None(Error *e);


/* Runtime */

void Initialize(void);
void Finalize(void);

void Open(void);
void Close(void);

void Try(void (*func)(void *arg), void *arg, Error *err);
void Panic(String *fmt, ...) __attribute__((noreturn));
void Defer(void (*func)(void *arg), void *arg);
void Trap(void (*func)(void *arg), void *arg);

/* Logging */

void Debug(String *fmt, ...);
void Info(String *fmt, ...);


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


/* Assertions */

#define Assert(expr) \
    Assert_((expr) == True, S(#expr), S(__func__), S(__FILE__), __LINE__)

void Assert_(Bool test, String *expr, String *func, String *file, Int line);

#define TODO() \
    Panic(S("not implemented: function %s, file \"%s\", line %d"), \
          S(__func__), S(__FILE__), __LINE__)

#endif /* PRELUDE_H */
