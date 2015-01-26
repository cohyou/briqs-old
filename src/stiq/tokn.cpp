#include "tokn.h"

Tokn::Tokn(ToknType t, string n, Baseplate *plate)
    : Text(n, plate) {
    d.sbtp1 = t;
}

unsigned char Tokn::t() {
    return d.sbtp1;
}

string Tokn::tree() {
    return indent() + "TOKN<" + name() + ">";
}

string Tokn::to_s() {
    return "TOKN<" + name() + ">";
}
