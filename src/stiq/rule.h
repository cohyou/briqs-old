#ifndef RULE_H
#define RULE_H

#include <string>
#include "cell.h"

using namespace std;

class Rule : public Cell {
public:
    Rule(string n, Baseplate *plate);
    string tree();
    string name();
};

#endif
