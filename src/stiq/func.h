#ifndef FUNC_H
#define FUNC_H

#include "cell.h"

enum FuncType { ATOM = 0x20, EQL_, CAR_, CDR_, CONS, };

class Func : public Cell {
public:
    Func(FuncType t, Baseplate *plate);
    string to_s();
};

#endif
