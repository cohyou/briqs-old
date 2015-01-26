#include "sval.h"

class Ui64 : public Sval {
public:
    Ui64(unsigned long ul, Baseplate *plate);
    string name();
    string tree();
    string to_s();
};
