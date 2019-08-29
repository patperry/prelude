#include "prelude/string.h"
#include "prelude/test.h"

/*
static Bytes *invalidUtf8(Bytes *b, Int n) {
    Bytes pre;
    Bytes_Gen(&pre, n / 2);
    Defer(Bytes_Drop, &pre);

    Bytes post;
    Bytes_Gen(&post, n / 2);
    Defer(Bytes_Drop, &post);

    Byte x = Byte_Choose(0x80, 0xff);

    BytesBuilder build = BytesBuilder_Init;
    Defer(BytesBuilder_Drop, &build);

    BytesBuilder_WriteBytes(&build, &pre);
    BytesBuilder_WriteByte(&build, x);
    BytesBuilder_WriteBytes(&build, &post);

    BytesBuilder_ToBytes(&build, b);
}
*/

static void prop_view_utf8_bytes(void) {
    Open();
    String s;
    Test_GenString(&s);
    Defer(String_Drop, &s);

    Error err = Error_Init;
    StringView v;
    String_ViewBytes(&v, &s.bytes, &err);
    Assert_ErrorNone(&err);
    Close();
}


int main(int argc, const char **argv) {
    Initialize();
    Open();

    TestSuite suite = TestSuite_Init;
    Defer(TestSuite_Drop, &suite);

    Int g = TestSuite_Group(&suite, S("string"));
    TestSuite_Property(&suite, g, S("view utf8 bytes"), prop_view_utf8_bytes);

    int ret = Test_Main(argc, argv, &suite);
    Close();
    Finalize();
    return ret;
}
