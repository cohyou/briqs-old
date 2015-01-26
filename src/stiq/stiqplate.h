#ifndef STIQPLATE_H
#define STIQPLATE_H

#include "baseplate.h"
#include "tokn.h"
#include "rule.h"
#include "spfm.h"
#include "func.h"
#include "smbl.h"

class Stiqplate : public Baseplate {
public:
    Tokn *make_tokn(ToknType t, string n);
    Rule *make_rule(string s);
    Spfm *make_spfm(SpfmType t);
    Func *make_func(FuncType t);
    Smbl *make_smbl(Tokn *t);
public:
    Bucket *make_bucket(string bucket_name);
};

#endif
