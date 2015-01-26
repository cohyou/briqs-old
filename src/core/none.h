#include "sval.h"

class None : public Sval {
public:
    None(Baseplate *plate);
    string to_s();
};
