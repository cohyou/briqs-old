#include "sval.h"

class Tval : public Sval {
public:
    Tval(Baseplate *plate);
    string tree();
    string to_s();
};
