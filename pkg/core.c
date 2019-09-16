#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "prelude.h"
#include "prelude/rng.h"
#include "prelude/string.h"

typedef enum {
    Finalizer_None = 0,
    Finalizer_Defer,
    Finalizer_Trap
} FinalizerType;

typedef struct Finalizer {
    void (*func)(void *arg);
    void *arg;
    FinalizerType type;
} Finalizer;

Define_Array(Finalizer)
Implement_Array(Finalizer)

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
    jmp_buf env;
    Error *err;
    Int scope;
    Int scopes_len;
} TryPoint;

#define TryPoint_Init (TryPoint){{0}, NULL, Int_None}

Define_Array(TryPoint)
Implement_Array(TryPoint)

typedef struct {
    Int max;
    Int cur;
} MemoryStats;

#define MemoryStats_Init (MemoryStats){0, 0}

typedef struct Runtime {
    Bool panic_allowed;
    MemoryStats memory;
    LiteralsArray literals;
    FinalizerArray finalizers;
    TryPointArray tries;
    IntArray scopes;
    StringBuilder log_builder;
    RngMaker rng_maker;
    Rng rng;
} Runtime;

static void Runtime_Drop(void *arg);

static Runtime runtime;

void Runtime_New(Runtime *r) {
    r->panic_allowed = False;
    r->memory = MemoryStats_Init;
    r->literals = LiteralsArray_Init;
    r->finalizers = Array_Init(Finalizer);
    r->tries = Array_Init(TryPoint);
    r->scopes = Array_Init(Int);
    r->log_builder = StringBuilder_Init;
    RngMaker_New(&r->rng_maker, RngMaker_None);
    RngMaker_Make(&r->rng_maker, &r->rng);
    r->panic_allowed = True;
}

void Runtime_Drop(void *arg) {
    Runtime *r = arg;
    Rng_Drop(&r->rng);
    RngMaker_Drop(&r->rng_maker);
    StringBuilder_Drop(&r->log_builder);
    IntArray_Drop(&r->scopes);
    TryPointArray_Drop(&r->tries);
    FinalizerArray_Drop(&r->finalizers);
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
    Runtime_New(&runtime);
}

void Finalize(void) {
    if (runtime.scopes.len) {
        fprintf(stderr, "[ERROR] stack imbalance: %d frame(s)\n",
                (int)runtime.scopes.len);
    }
    Runtime_Drop(&runtime);
    if (runtime.memory.cur) {
        fprintf(stderr, "[ERROR] memory leak: %zu bytes\n",
                (size_t)runtime.memory.cur);
    }
}

void Open(void) {
    Int scope = runtime.finalizers.len;
    IntArray_Grow(&runtime.scopes, 1);
    runtime.scopes.items[runtime.scopes.len++] = scope;
    //Debug(S("push scope %d"), scope);
}

static void unroll(Int scope, Bool trap) {
    while (runtime.finalizers.len != scope) {
        Assert(runtime.finalizers.len >= scope);

        Finalizer action = runtime.finalizers.items[--runtime.finalizers.len];
        switch (action.type) {
        case Finalizer_None:
            break;

        case Finalizer_Trap:
            if (trap) {
                action.func(action.arg);
            }
            break;

        case Finalizer_Defer:
            action.func(action.arg);
            break;
        }
    }
}

void Close(void) {
    Assert(runtime.scopes.len > 0);
    Int scope = runtime.scopes.items[--runtime.scopes.len];
    //Debug(S("pop scope %d"), scope);
    unroll(scope, False);
}

void Try(void (*func)(void *arg), void *arg, Error *err) {
    TryPointArray_Grow(&runtime.tries, 1);
    Int n = runtime.tries.len;
    runtime.tries.items[n].err = err;
    runtime.tries.items[n].scope = runtime.finalizers.len;
    runtime.tries.items[n].scopes_len = runtime.scopes.len;
    if (!setjmp(runtime.tries.items[n].env)) {
        runtime.tries.len = n + 1;
        func(arg);
    }
    runtime.tries.len = n;
}

void Panic(String *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    Error err;
    Error_NewFromArgList(&err, fmt, ap);
    va_end(ap);

    Int n = runtime.tries.len;
    if (n) {
        *runtime.tries.items[n - 1].err = err;
        Int s = runtime.tries.items[n - 1].scope;
        Int l = runtime.tries.items[n - 1].scopes_len;
        unroll(s, True);
        runtime.scopes.len = l;
        longjmp(runtime.tries.items[n - 1].env, 1);
    } else {
        fprintf(stderr, "panic: %.*s\n", (int)err.string.bytes.len,
                (const char *)err.string.bytes.ptr);
        Error_Drop(&err);
        unroll(0, True);
        abort();
    }
}

static void addFinalizer(FinalizerType type, void (*func)(void *arg),
                         void *arg) {
    FinalizerArray_Grow(&runtime.finalizers, 1);
    Int n = runtime.finalizers.len;
    runtime.finalizers.items[n] = (Finalizer){
        .func = func,
        .arg = arg,
        .type = type
    };
    runtime.finalizers.len = n + 1;
}

void Defer(void (*func)(void *arg), void *arg) {
    addFinalizer(Finalizer_Defer, func, arg);
}

void Trap(void (*func)(void *arg), void *arg) {
    addFinalizer(Finalizer_Trap, func, arg);
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
    // TODO: handle negative from, to; can't use Assert because it allocates

    runtime.memory.cur -= from;
    if (to == 0) {
        free(ptr);
        return NULL;
    }

    ptr = realloc(ptr, (size_t)to);
    if (!ptr) {
        // TODO: avoid allocation on panic
        Panic(S("failed allocating %d bytes"), to);
    }
    runtime.memory.cur += to;
    if (runtime.memory.cur > runtime.memory.max) {
        runtime.memory.max = runtime.memory.cur;
    }

    return ptr;
}

void Log(LogType type, String *fmt, va_list ap) {
    StringBuilder_WriteFormatArgList(&runtime.log_builder, fmt, ap);

    StringView s;
    StringBuilder_View(&runtime.log_builder, &s);
    FILE *stream;
    const char *tag;

    if (type == Log_Info) {
        stream = stdout;
        tag = "";
    } else {
        stream = stderr;
        tag = "[DEBUG] ";
    }

    time_t clock;
    struct tm tm;

    time(&clock);
    tm = *gmtime(&clock); // TODO: replace with reentrant version

    fprintf(stream, "%d-%02d-%02d %02d:%02d:%02d %s%.*s\n",
            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
            tm.tm_hour, tm.tm_min, tm.tm_sec, tag,
            (int)s.string.bytes.len, s.string.bytes.ptr);
    fflush(stream);

    StringBuilder_Clear(&runtime.log_builder);
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

Rng *System_Rng(void) {
    return &runtime.rng;
}

RngMaker *System_RngMaker(void) {
    return &runtime.rng_maker;
}
