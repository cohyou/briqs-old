#include "fval.h"

Fval::Fval(Baseplate *plate)
    : Sval(FVAL, plate) {}

string Fval::tree() {
    return indent() + "SVAL[FVAL]";
}

string Fval::to_s() {
    return "F";
}
