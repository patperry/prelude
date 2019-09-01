#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "prelude.h"
#include "prelude/string.h"

// Don't use array for literals, to avoid circular dependency

enum {
    Literals_Cap = 255
};

typedef struct Literals {
    Int len;
    Bytes items[Literals_Cap];
} Literals;

#define Literals_Init (Literals){0}

typedef struct LiteralsArray {
    Literals *items;
    Int len;
    Int cap;
} LiteralsArray;

#define LiteralsArray_Init (LiteralsArray){NULL, 0, 0}

static void LiteralsArray_Drop(void *arg);
static void LiteralsArray_Grow(LiteralsArray *a);
static Bytes *LiteralsArray_Push(LiteralsArray *a, Bytes *x);

typedef enum LogType {
    Log_None = 0,
    Log_Debug,
    Log_Info
} LogType;

static void Log(LogType type, String *fmt, va_list ap);

typedef struct TryPoint {
    Error *err;
    jmp_buf env;
} TryPoint;

#define TryPoint_Init (TryPoint){NULL, {0}}

typedef struct Runtime {
    LiteralsArray literals;
    TryPoint try;
    Bool has_try;
} Runtime;

#define Runtime_Init (Runtime){LiteralsArray_Init, TryPoint_Init, False}

static void Runtime_Drop(void *arg);

static Runtime runtime;

void Runtime_Drop(void *arg) {
    Runtime *r = arg;
    LiteralsArray_Drop(&r->literals);
}

void LiteralsArray_Drop(void *arg) {
    LiteralsArray *a = arg;
    Free(a->items, a->cap * SizeOf(*a->items));
}

void LiteralsArray_Grow(LiteralsArray *a) {
    Int n = a->len;
    if (n == 0 || a->items[n - 1].len == Literals_Cap) {
        if (a->len == a->cap) {
            a->items = Realloc(a->items, n * SizeOf(*a->items),
                               (n + 1) * SizeOf(*a->items));
            a->cap = n + 1;
        }
        a->items[n] = Literals_Init;
        a->len = n + 1;
    }
}

Bytes *LiteralsArray_Push(LiteralsArray *a, Bytes *x) {
    LiteralsArray_Grow(a);
    Literals *l = &a->items[a->len - 1];
    Bytes *item = &l->items[l->len++];
    *item = *x;
    return item;
}


void Initialize(void) {
    runtime = Runtime_Init;
}

void Finalize(void) {
    Runtime_Drop(&runtime);
}

void Open(void) {
}

void Close(void) {
}

void Try(void (*func)(void *arg), void *arg, Error *err) {
    Assert(!runtime.has_try); // TODO: nested try not implemented
    runtime.try.err = err;
    runtime.has_try = True;
    if (!setjmp(runtime.try.env)) {
        func(arg);
    }
    runtime.has_try = False;
}

void Panic(String *fmt, ...) {
    // TODO: careful error handling
    va_list ap;
    va_start(ap, fmt);

    StringBuilder b = StringBuilder_Init;
    Defer(StringBuilder_Drop, &b);
    StringBuilder_WriteFormatArgList(&b, fmt, ap);
    va_end(ap);

    if (runtime.has_try == True) {
        StringBuilder_ToString(&b, &runtime.try.err->string);
        longjmp(runtime.try.env, 1);
    } else {
        String msg;
        StringBuilder_ToString(&b, &msg);
        fprintf(stderr, "panic: %.*s\n", (int)msg.bytes.len,
                (const char *)msg.bytes.ptr);
        String_Drop(&msg);
        abort();
    }
}

void Defer(void (*func)(void *arg), void *arg) {
    (void)func;
    (void)arg;
    // TODO
}

void Trap(void (*func)(void *arg), void *arg) {
    (void)func;
    (void)arg;
    // TODO
}

String *S(const char *str) {
    // TODO: validation
    return (String *)B(str);
}

Bytes *B(const char *str) {
    Int i, n = runtime.literals.len;
    for (i = 0; i < n; i++) {
        Literals *l = &runtime.literals.items[i];
        Int j, m = l->len;
        for (j = 0; j < m; j++) {
            if ((const char *)l->items[j].ptr == str) {
                return &l->items[j];
            }
        }
    }

    Bytes b = {
        .ptr = (Byte *)str,
        .len = (Int)strlen(str)
    };
    return LiteralsArray_Push(&runtime.literals, &b);
}

void Memory_Setup(Memory *mem, Int len) {
    mem->ptr = Alloc(len);
    mem->len = len;
}

void Memory_Teardown(void *arg) {
    Memory *mem = arg;
    Free(mem->ptr, mem->len);
}

void *Alloc(Int len) {
    return Realloc(NULL, 0, len);
}

void Free(void *ptr, Int len) {
    Realloc(ptr, len, 0);
}

void *Realloc(void *ptr, Int from, Int to) {
    (void)from;

    if (to <= 0) {
        free(ptr);
        return NULL;
    }

    ptr = realloc(ptr, (size_t)to);
    if (!ptr) {
        // TODO: avoid allocation on panic
        Panic(S("failed allocating %d bytes"), to);
    }

    return ptr;
}

void Log(LogType type, String *fmt, va_list ap) {
    StringBuilder b = StringBuilder_Init;
    Defer(StringBuilder_Drop, &b);

    StringBuilder_WriteFormatArgList(&b, fmt, ap);

    String s;
    StringBuilder_ToString(&b, &s); // TODO: use shared builder for all logs
    Defer(String_Drop, &s);

    if (type == Log_Info) {
        fprintf(stdout, "%.*s\n", s.bytes.len, s.bytes.ptr);
        fflush(stdout);
    } else if (type == Log_Debug) {
        time_t clock;
        struct tm tm;

        time(&clock);
        tm = *gmtime(&clock); // TODO: replace with reentrant version

        fprintf(stderr, "%d-%02d-%02d %02d:%02d:%02d [DEBUG] %.*s\n",
                tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
                tm.tm_hour, tm.tm_min, tm.tm_sec,
                s.bytes.len, s.bytes.ptr);
        fflush(stderr);
    }
}

void Debug(String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Log(Log_Debug, fmt, ap);
    va_end(ap);
}

void Info(String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Log(Log_Info, fmt, ap);
    va_end(ap);
}
