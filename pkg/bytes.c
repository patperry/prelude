#include <string.h>
#include "prelude/bytes.h"

void Bytes_Teardown(void *arg) {
    Bytes *b = arg;
    (void)b;
    TODO();
}

Bool Bytes_None(Bytes *b) {
    return b->ptr == NULL;
}

Bool Bytes_Some(Bytes *b) {
    return b->ptr != NULL;
}

void Bytes_Split(Bytes *b, Bytes *sep, BytesView *head, BytesView *tail) {
    if (Bytes_None(sep)) {
        head->bytes = (Bytes){};
        tail->bytes = *b;
        return;
    }

    Int i, n = b->len;
    Int j, m = sep->len;

    for (i = 0; i < (n - m) + 1; i++) {
        if (b->ptr[i] != sep->ptr[0]) {
            continue;
        }
        for (j = 1; j < m; j++) {
            if (b->ptr[i + j] != sep->ptr[j]) {
                break;
            }
        }
        if (j == m) {
            head->bytes.ptr = b->ptr;
            head->bytes.len = i;
            tail->bytes.ptr = b->ptr + i + m;
            tail->bytes.len = b->len - i - m;
            return;
        }
    }

    head->bytes = *b;
    tail->bytes = (Bytes){};
}

void BytesBuilder_Setup(BytesBuilder *b) {
    *b = (BytesBuilder){};
}

void BytesBuilder_Teardown(void *arg) {
    BytesBuilder *b = arg;
    ByteArray_Teardown(&b->buf);
}

void BytesBuilder_WriteByte(BytesBuilder *b, Byte x) {
    ByteArray_Grow(&b->buf, 1);
    b->buf.items[b->buf.len++] = x;
}

void BytesBuilder_WriteBytes(BytesBuilder *b, Bytes *xs) {
    Int n = xs->len;
    ByteArray_Grow(&b->buf, n);
    memcpy(b->buf.items + b->buf.len, xs->ptr, n);
    b->buf.len += n;
}

void Bytes_SetupWithBuilder(Bytes *xs, BytesBuilder *b) {
    xs->ptr = Realloc(b->buf.items, b->buf.cap, b->buf.len);
    xs->len = b->buf.len;
    b->buf = (ByteArray){};
}
