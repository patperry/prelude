#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "prelude.h"
#include "prelude/string.h"

// Don't use array for literals, to avoid circular dependency

enum {
    Literals_Cap = 1024
};

typedef struct Literals {
    Bytes *items; // never reallocate, so literals have stable pointers
    Int len;
    Int cap;
} Literals;

static void Literals_Setup(Literals *l) {
    l->len = 0;
    l->cap = Literals_Cap;
    l->items = Alloc(l->cap * SizeOf(*l->items));
}

static void Literals_Teardown(void *arg) {
    Literals *l = arg;
    Free(l->items, l->cap * SizeOf(*l->items));
}

typedef struct LiteralsArray {
    Literals *items;
    Int len;
    Int cap;
} LiteralsArray;

static void LiteralsArray_Setup(LiteralsArray *a) {
    a->cap = 0;
    a->len = 0;
    a->items = NULL;
}

static void LiteralsArray_Teardown(void *arg) {
    LiteralsArray *a = arg;
    Int n = a->len;
    while (n-- > 0) {
        Literals_Teardown(&a->items[n]);
    }
    Free(a->items, a->cap * SizeOf(*a->items));
}

static void LiteralsArray_Grow(LiteralsArray *a) {
    Int n = a->len;
    if (n == 0 || a->items[n - 1].len == a->items[n - 1].cap) {
        if (a->len == a->cap) {
            a->items = Realloc(a->items, n * SizeOf(*a->items),
                               (n + 1) * SizeOf(*a->items));
            a->cap = n + 1;
        }
        Literals_Setup(&a->items[n]);
        a->len = n + 1;
    }
}

static Bytes *LiteralsArray_Push(LiteralsArray *a, Bytes *x) {
    LiteralsArray_Grow(a);
    Literals *l = &a->items[a->len - 1];
    Bytes *item = &l->items[l->len++];
    *item = *x;
    return item;
}

typedef struct Runtime {
    LiteralsArray literals;
} Runtime;

static Runtime runtime;

void Initialize(void) {
    LiteralsArray_Setup(&runtime.literals);
}

void Finalize(void) {
    LiteralsArray_Teardown(&runtime.literals);
}

void Open(void) {
}

void Close(void) {
}

typedef enum LogType {
    Log_None = 0,
    Log_Debug,
    Log_Info
} LogType;

static void Log(LogType type, String *fmt, va_list ap) {
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

void Panic(String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    Error err;
    StringBuilder b = StringBuilder_Init;
    Defer(StringBuilder_Drop, &b);
    StringBuilder_WriteFormatArgList(&b, fmt, ap);
    StringBuilder_ToString(&b, &err.string);

    fprintf(stderr, "panic: %.*s\n", (int)err.string.bytes.len,
            (const char *)err.string.bytes.ptr);
    // TODO persist error
    // TODO memory leak (err)

    va_end(ap);
    abort();
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
