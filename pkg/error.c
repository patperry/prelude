#include "prelude.h"

void Error_Drop(void *arg) {
    Error *e = arg;
    String_Drop(&e->string);
}

Bool Error_None(Error *e) {
    return String_None(&e->string);
}

Bool Error_Some(Error *e) {
    return String_Some(&e->string);
}
