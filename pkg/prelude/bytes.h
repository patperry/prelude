#ifndef PRELUDE_BYTES_H
#define PRELUDE_BYTES_H

#include "prelude.h"
#include "prelude/array.h"

typedef struct BytesBuilder {
    ByteArray buf;
} BytesBuilder;
#define BytesBuilder_Init (BytesBuilder){Array_Init(Byte)}

void BytesBuilder_Drop(void *arg);
void BytesBuilder_WriteByte(BytesBuilder *b, Byte x);
void BytesBuilder_WriteBytes(BytesBuilder *b, Bytes *s);

void BytesBuilder_ToBytes(BytesBuilder *b, Bytes *s);


typedef struct BytesView {
    Bytes bytes;
} BytesView;

Bool Bytes_UnCons(Bytes *s, Byte *head, BytesView *tail);
void Bytes_Split(Bytes *s, Bytes *sep, BytesView *head, BytesView *tail);

#endif /* PRELUDE_BYTES_H */
