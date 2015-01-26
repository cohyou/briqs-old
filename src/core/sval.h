#ifndef SVAL_H
#define SVAL_H

#include "briq.h"

enum SvalType {
    NONE = 0xC0,
    FVAL = 0xC2, TVAL = 0xC3,
    BIN_ = 0xC4, BIN2 = 0xC5,
    FL32 = 0xCA, FL64 = 0xCB,
    UI08 = 0xCC, UI16 = 0xCD, UI32 = 0xCE, UI64 = 0xCF,
    IT08 = 0xD0, IT16 = 0xD1, IT32 = 0xD2, IT64 = 0xD3,
};

class Sval : public Briq {
public:
    Sval(SvalType t, Baseplate *plate);
    string tree();
    string to_s();
private:
    string get_sval_type_name();
};

#endif
