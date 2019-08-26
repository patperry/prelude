#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "prelude.h"
#include "prelude/array.h"

enum {
    LiteralBuffer = 32
};

typedef struct Runtime {
    BytesArray literals;
} Runtime;

static Runtime runtime;

void Initialize(void) {
    BytesArray_Setup(&runtime.literals);
    Trap(BytesArray_Teardown, &runtime.literals);
    BytesArray_Grow(&runtime.literals, LiteralBuffer);
}

void Finalize(void) {
    BytesArray_Teardown(&runtime.literals);
}

void Open(void) {
}

void Close(void) {
}

void Panic(String *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);
    Error err;
    Error_SetupWithArgList(&err, fmt, ap);
    va_end(ap);

    fprintf(stderr, "panic: %.*s\n", (int)err.string.bytes.len,
            (const char *)err.string.bytes.ptr);

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
    BytesArray_Grow(&runtime.literals, 1);
    Int n = runtime.literals.len++;
    runtime.literals.items[n] = (Bytes){
        .ptr = (Byte *)str,
        .len = (Int)strlen(str)
    };
    return &runtime.literals.items[n];
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
        Panic(S("failed allocating %d bytes"), to);
    }

    return ptr;
}
