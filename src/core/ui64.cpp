#include <sstream>
#include "ui64.h"

Ui64::Ui64(unsigned long ul, Baseplate *plate)
    : Sval(UI64, plate) {
    d.lval = ul;
}

string Ui64::name() {
    return to_s();
}

string Ui64::tree() {
    stringstream ss;
    ss << indent() << "UI64[" << d.lval << "]";
    return ss.str();
}

string Ui64::to_s() {
    stringstream ss;
    ss << d.lval;
    return ss.str();
}
