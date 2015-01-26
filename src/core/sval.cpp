#include <iostream>
#include "sval.h"

Sval::Sval(SvalType t, Baseplate *plate)
    : Briq(SVAL, plate) {
    d.type = t;
}

string Sval::tree() {
    return indent() + "SVAL[" + get_sval_type_name() + "]";
}

string Sval::to_s() {
    return "SVAL<" + get_sval_type_name() + ">";
}

string Sval::get_sval_type_name() {
    static const string type_names[] = {
        "NONE",
        "FVAL", "TVAL",
        "BIN_", "BIN2",
        "FL32", "FL64",
        "UI08", "UI16", "UI32", "UI64",
        "IT08", "IT16", "IT32", "IT64",
        "TXT_", "TXT2",
    };

    switch (d.type) {
        case 0x00: return "INVL";
        case NONE: return type_names[0];
        case FVAL: return type_names[1];
        case TVAL: return type_names[2];
        case BIN_: return type_names[3];
        case BIN2: return type_names[4];
        case FL32: return type_names[5];
        case FL64: return type_names[6];
        case UI08: return type_names[7];
        case UI16: return type_names[8];
        case UI32: return type_names[9];
        case UI64: return type_names[10];
        case IT08: return type_names[11];
        case IT16: return type_names[12];
        case IT32: return type_names[13];
        case IT64: return type_names[14];
        default:
        cout << "!!!! INVL TYPE !!!! <" << d.type << ">" << endl;
    }
    return "";
}
