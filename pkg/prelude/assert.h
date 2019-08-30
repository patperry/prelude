#ifndef PRELUDE_ASSERT_H
#define PRELUDE_ASSERT_H

#include "prelude.h"

#define Assert_ErrorNone(err) \
    Assert_ErrorNone_(err, S(__func__), S(__FILE__), __LINE__)
void Assert_ErrorNone_(Error *err, String *func, String *file, Int line);

#define Assert_StringEq(s1, s2) \
    Assert_StringEq_(s1, s2, S(__func__), S(__FILE__), __LINE__)
void Assert_StringEq_(String *s1, String *s2, String *func, String *file,
                      Int line);

#define Assert_StringNotEq(s1, s2) \
    Assert_StringNotEq_(s1, s2, S(__func__), S(__FILE__), __LINE__)
void Assert_StringNotEq_(String *s1, String *s2, String *func, String *file,
                         Int line);

#endif /* PRELUDE_ASSERT_H */
