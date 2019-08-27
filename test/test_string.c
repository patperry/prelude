#include "prelude/string.h"
#include "prelude/test.h"


static void prop_view_utf8_bytes(void) {
    String *s = Test_GenString();

    StringView v;
    Error err = {};
    String_ViewBytes(&v, &s->bytes, &err);
    Assert_ErrorNone(&err);
}


int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestSuite suite;
    TestSuite_Setup(&suite);
    Defer(TestSuite_Teardown, &suite);

    Int g = TestSuite_Group(&suite, S("string"));
    TestSuite_Property(&suite, g, S("view utf8 bytes"), prop_view_utf8_bytes);

    int ret = Test_Main(argc, argv, &suite);
    Close();
    Finalize();
    return ret;
}
