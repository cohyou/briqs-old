#include "cell.h"
#include "tokn.h"

class Smbl : public Cell {
public:
    Smbl(Baseplate *plate);
    Smbl(Tokn *t, Baseplate *plate);
    string tree();
    string to_s();
    string name();
};
