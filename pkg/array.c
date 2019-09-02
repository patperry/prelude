#include "prelude.h"
#include "prelude/array.h"

Implement_Array(Byte)
Implement_Array(Bytes)
Implement_Array(Int)


enum {
    Array_Init = 32
};


Int Array_NextCap(Int elt, Int cap, Int diff) {
    Assert(cap >= 0);

    if (elt <= 0 || diff <= 0) {
        return cap;
    }

    if (cap > Int_Max - diff) {
        Panic(S("required array capacity exceeds maximum (%d)"), Int_Max);
    }

    Int min = cap + diff;
    if (min > Int_Max / elt) {
        Panic(S("required array capacity exceeds maximum (%d)"),
              Int_Max / elt);
    }

    if (cap == 0) {
        cap = Array_Init;
    }

    while (cap < min) {
        if (cap > Int_Max / 2) {
            return min;
        }
        cap *= 2;
    }

    if (cap > Int_Max / elt) {
        return min;
    }

    return cap;
}
