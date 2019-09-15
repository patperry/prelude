#ifndef PRELUDE_ARRAY_H
#define PRELUDE_ARRAY_H

#define Array(T) T ## Array
#define Array_Member(T, M) T ## Array_ ## M

#define Define_Array(T) \
    typedef struct Array(T) { \
        T *items; \
        Int len; \
        Int cap; \
    } Array(T); \
    void Array_Member(T, Drop)(void *arg); \
    void Array_Member(T, Grow)(Array(T) *x, Int buffer);

#define Array_Init(T) (Array(T)){NULL, 0, 0}

Int Array_NextCap(Int elt, Int cap, Int diff);

#define Implement_Array(T) \
    void Array_Member(T, Drop)(void *arg) { \
        Array(T) *x = arg; \
        Free(x->items, x->cap * SizeOf(T)); \
    } \
    void Array_Member(T, Grow)(Array(T) *x, Int n) { \
        if (n <= 0) { \
            return; \
        } \
        Int diff = n - (x->cap - x->len); \
        if (diff <= 0) { \
            return; \
        } \
        Int cap1 = Array_NextCap(SizeOf(T), x->cap, diff); \
        x->items = Realloc(x->items, x->cap * SizeOf(T), cap1 * SizeOf(T)); \
        x->cap = cap1; \
    }

Define_Array(Byte)
Define_Array(Bytes)
Define_Array(Int)
Define_Array(Word64)

#endif /* PRELUDE_ARRAY_H */
