#include "sval.h"

class Fval : public Sval {
public:
    Fval(Baseplate *plate);
    string tree();
    string to_s();
};
