#include "tval.h"

Tval::Tval(Baseplate *plate)
    : Sval(TVAL, plate) {}

string Tval::tree() {
    return indent() + "SVAL[TVAL]";
}

string Tval::to_s() {
    return "T";
}
