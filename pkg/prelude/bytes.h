#ifndef PRELUDE_BYTES_H
#define PRELUDE_BYTES_H

#include "prelude.h"
#include "prelude/array.h"

typedef struct BytesBuilder {
    ByteArray buf;
} BytesBuilder;

void BytesBuilder_Setup(BytesBuilder *b);
void BytesBuilder_Teardown(void *arg);
void BytesBuilder_WriteByte(BytesBuilder *b, Byte x);
void BytesBuilder_WriteBytes(BytesBuilder *b, Bytes *xs);

void Bytes_SetupWithBuilder(Bytes *xs, BytesBuilder *b);


typedef struct BytesView {
    Bytes bytes;
} BytesView;

void Bytes_Split(Bytes *s, Bytes *sep, BytesView *head, BytesView *tail);

#endif /* PRELUDE_BYTES_H */
