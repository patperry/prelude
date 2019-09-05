#include "prelude/assert.h"

void Assert_Panic_(void (*func)(void *arg), void *arg,
                   String *expr, String *funcs, String *file, Int line)
{
    Error err = Error_Init;
    Try(func, arg, &err);
    if (Error_Some(&err)) {
        Error_Drop(&err);
    } else {
        Panic(S("expected panic from %s: function %s, file \"%s\", line %d"),
              expr, funcs, file, line);
    }
}

void Assert_NotPanic_(void (*func)(void *arg), void *arg,
                      String *expr, String *funcs, String *file, Int line)
{
    Open();
    Error err = Error_Init;
    Try(func, arg, &err);
    if (Error_Some(&err)) {
        Defer(Error_Drop, &err);
        Panic(S("unexpected panic \"%s\" from %s"
                ": function %s, file \"%s\", line %d"),
              &err, expr, funcs, file, line);
    }
    Close();
}

void Assert_(Bool test, String *expr, String *func, String *file, Int line) {
    if (test != True) {
         Panic(S("assertion \"%s\" failed: function %s, file \"%s\", line %d"),
               expr, func, file, line);
    }
}

void Assert_ErrorNone_(Error *err, String *func, String *file, Int line) {
    if (Error_Some(err)) {
        Error_Drop(err);
        Panic(S("unexpected error: function %s, file \"%s\", line %d"),
              func, file, line);
    }
}

void Assert_IntEq_(Int x1, Int x2, String *func, String *file, Int line) {
    if (x1 != x2) {
        Panic(S("assertion %d = %d failed"
                ": function %s, file \"%s\", line %d"),
              x1, x2, func, file, line);
    }
}

void Assert_IntNotEq_(Int x1, Int x2, String *func, String *file, Int line) {
    if (x1 == x2) {
        Panic(S("assertion %d != %d failed"
                ": function %s, file \"%s\", line %d"),
              x1, x2, func, file, line);
    }
}

void Assert_StringEq_(String *s1, String *s2, String *func, String *file,
                      Int line) {
    if (String_Eq(s1, s2) != True) {
        Panic(S("assertion \"%s\" = \"%s\" failed"
                ": function %s, file \"%s\", line %d"),
              s1, s2, func, file, line);
    }
}

void Assert_StringNotEq_(String *s1, String *s2, String *func, String *file,
                         Int line) {
    if (String_Eq(s1, s2) != False) {
        Panic(S("assertion \"%s\" != \"%s\" failed"
                ": function %s, file \"%s\", line %d"),
              s1, s2, func, file, line);
    }
}
