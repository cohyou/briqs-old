#include "scope.h"

Scope::Scope() : name("global"), enclosingScope(NULL) {}
Scope::Scope(string n, Scope *parent) : name(n), enclosingScope(parent) {}

string Scope::getScopeName() { return name; }
Scope *Scope::getEnclosingScope() { return enclosingScope; }
void Scope::define(string sym, Briq *content) { symbolTable[sym] = content; }
Briq *Scope::resolve(string name) {
    Briq *result;
    result = symbolTable[name];
    if (result != NULL) return result;
    if (enclosingScope != NULL) return enclosingScope->resolve(name);
    cout << "!!!! CANT RSLV !!!! NAME: [" << name << "]" << endl;
    return NULL;
}
