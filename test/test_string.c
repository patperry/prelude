#include "prelude/string.h"
#include "prelude/test.h"


static void prop_view_utf8_bytes(void) {
    String *s = Test_GenString();

    StringView v;
    Error err = {0};
    String_ViewBytes(&v, &s->bytes, &err);
    Assert_ErrorNone(&err);
}


int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestGroup string;
    TestGroup_Setup(&string, S("string"));
    Defer(TestGroup_Teardown, &string);
    TestGroup_Property(&string, S("view utf8 bytes"), prop_view_utf8_bytes);

    TestSuite suite;
    TestSuite_Setup(&suite);
    Defer(TestSuite_Teardown, &suite);
    TestSuite_Add(&suite, &string);

    int ret = Test_Main(argc, argv, &suite);

    Close();
    Finalize();
    return ret;
}
