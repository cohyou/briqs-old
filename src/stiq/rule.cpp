#include <iostream>
#include <sstream>
#include "rule.h"

using namespace std;

Rule::Rule(string n, Baseplate *plate)
    : Cell(RULE, plate) {
    if (n.size() > 8) {
        return;
    }
    const char *c_str = n.c_str();
    for (size_t i = 0; i < n.size(); ++i) {
        d.data2[i] = c_str[i];
    }
}

string Rule::tree() {
    return tree_impl("RULE<" + name() + ">");
}

string Rule::to_s() {
    stringstream ss;
    if (name() != "ROOT") {
        ss << "(";
    }
    ss << to_s_impl();
    return ss.str();
}

string Rule::name() {
    string s;
    for (int i = 0; i < 8; ++i) {
        if (d.data2[i] != 0) {
            s += d.data2[i];
        }
    }
    return s;
}
