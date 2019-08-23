#include "prelude.h"
#include "string.h"
#include "test.h"


static Bool prop_view_utf8_bytes(void) {
    String *s = Test_String();

    StringView v;
    Error err;
    String_ViewBytes(&v, &s->bytes, &err);

    if (!Error_None(&err)) {
        Error_Teardown(&err);
        return False;
    }

    return True;
}

static Test string_tests[] = {
    {Test_Prop, "view utf8 bytes", prop_view_utf8_bytes},
    {Test_None, NULL, NULL}
};

static TestGroup tests[] = {
    {"string", string_tests},
    {NULL, NULL}
};

int main(int argc, const char **argv) {
    return Test_Main(argc, argv, tests);
}
