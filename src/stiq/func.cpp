#include <sstream>
#include "func.h"

Func::Func(FuncType t, Baseplate *plate)
    : Cell(FUNC, plate) {
    d.sbtp3 = t;
}

string Func::to_s() {
    stringstream ss;
    switch (d.sbtp3) {
        case ATOM: ss << "@"; break;
        case EQL_: ss << "="; break;
        case CAR_: ss << "L"; break;
        case CDR_: ss << "G"; break;
        case CONS: ss << "~"; break;
    }
    return ss.str();
}
