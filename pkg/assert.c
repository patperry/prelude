#include "prelude/assert.h"

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
