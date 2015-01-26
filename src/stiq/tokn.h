#ifndef TOKN_H
#define TOKN_H

#include "text.h"

enum ToknType { TOKN = 0x30, LBCT, RBCT, SGQT, DBQT, SMCL, EOF_, };

class Tokn : public Text {
public:
    Tokn(ToknType t, string n, Baseplate *plate);
    unsigned char t();
    string tree();
    string to_s();
};

#endif
