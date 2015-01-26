#ifndef SPFM_H
#define SPFM_H

#include "cell.h"

enum SpfmType { QUOT = 0x09, COND, LMBD, DEFN, };

class Spfm : public Cell {
public:
    Spfm(SpfmType t, Baseplate *plate);
    string tree();
    string to_s();
private:
    string get_spfm_type_name();
};

#endif
