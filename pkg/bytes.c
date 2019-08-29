#include <string.h>
#include "prelude/bytes.h"

void Bytes_FromCopy(Bytes *b, Bytes *other) {
    b->ptr = Alloc(other->len);
    b->len = other->len;
    memcpy(b->ptr, other->ptr, b->len);
}

void Bytes_Drop(void *arg) {
    Bytes *b = arg;
    Free(b->ptr, b->len);
}

Bool Bytes_None(Bytes *b) {
    return b->ptr == NULL;
}

Bool Bytes_Some(Bytes *b) {
    return b->ptr != NULL;
}

Bool Bytes_Eq(Bytes *b1, Bytes *b2) {
    if (b1->len != b2->len) {
        return False;
    } else if (memcmp(b1->ptr, b2->ptr, (size_t)b1->len) == 0) {
        return True;
    } else {
        return False;
    }
}

Bool Bytes_UnCons(Bytes *b, Byte *head, BytesView *tail) {
    if (Bytes_None(b)) {
        *head = Byte_None;
        tail->bytes = *b;
        return False;
    } else {
        *head = b->ptr[0];
        tail->bytes.ptr = b->ptr + 1;
        tail->bytes.len = b->len - 1;
        return True;
    }
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

void BytesBuilder_Drop(void *arg) {
    BytesBuilder *b = arg;
    ByteArray_Teardown(&b->buf);
}

void BytesBuilder_WriteByte(BytesBuilder *b, Byte x) {
    ByteArray_Grow(&b->buf, 1);
    b->buf.items[b->buf.len++] = x;
}

void BytesBuilder_WriteBytes(BytesBuilder *b, Bytes *s) {
    Int n = s->len;
    ByteArray_Grow(&b->buf, n);
    memcpy(b->buf.items + b->buf.len, s->ptr, n);
    b->buf.len += n;
}

void BytesBuilder_ToBytes(BytesBuilder *b, Bytes *s) {
    s->ptr = Realloc(b->buf.items, b->buf.cap, b->buf.len);
    s->len = b->buf.len;
    *b = BytesBuilder_Init;
}
