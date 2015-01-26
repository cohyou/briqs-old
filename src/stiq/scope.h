#ifndef SCOPE_H
#define SCOPE_H

#include <string>
#include <iostream>
#include <map>
#include "briq.h"

using namespace std;

class Scope {
    string name;
    Scope *enclosingScope;
    map<string, Briq *> symbolTable;
public:
    Scope();
    Scope(string name, Scope *parent);
    string getScopeName();
    Scope *getEnclosingScope();
    void define(string sym, Briq *content);
    Briq *resolve(string name);
};

#endif
