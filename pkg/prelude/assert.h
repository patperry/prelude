#ifndef PRELUDE_ASSERT_H
#define PRELUDE_ASSERT_H

#include "prelude.h"

#define Assert_ErrorNone(err) \
    Assert_ErrorNone_(err, S(__func__), S(__FILE__), __LINE__)

void Assert_ErrorNone_(Error *err, String *func, String *file, Int line);

#endif /* PRELUDE_ASSERT_H */
