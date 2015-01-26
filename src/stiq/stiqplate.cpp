#include <iostream>
#include "stiqplate.h"
#include "stiqbucket.h"

Tokn *Stiqplate::make_tokn(ToknType t, string n) {
    Tokn *tokn = new Tokn(t, n, this);
    briqs.push_back(tokn);
    // cout << "---- MAKE_TOKN ---- : " << hex << (void *)tokn << n << endl;
    return tokn;
}

Rule *Stiqplate::make_rule(string s) {
    Rule *rule = new Rule(s, this);
    briqs.push_back(rule);
    // cout << ""---- MAKE_RULE ---- : " << hex << (void *)rule << s << endl;
    return rule;
}

Spfm *Stiqplate::make_spfm(SpfmType t) {
    Spfm *spfm = new Spfm(t, this);
    briqs.push_back(spfm);
    // cout << ""---- MAKE_SPFM ---- : " << hex << (void *)spfm << endl;
    return spfm;
}

Func *Stiqplate::make_func(FuncType t) {
    Func *func = new Func(t, this);
    briqs.push_back(func);
    // cout << ""---- MAKE_FUNC ---- : " << hex << (void *)func << endl;
    return func;
}

Smbl *Stiqplate::make_smbl(Tokn *t) {
    Smbl *smbl = new Smbl(t, this);
    briqs.push_back(smbl);
    // cout << ""---- MAKE_SMBL ---- : " << hex << (void *)smbl << endl;
    return smbl;
}

Bucket *Stiqplate::make_bucket(string bucket_name) {
    Bucket *bucket = 0;

    map<string, Bucket *>::iterator it = buckets.find(bucket_name);
    if (it == buckets.end()) {
        bucket = new Stiqbucket(bucket_name, this);
        buckets[bucket_name] = bucket;
    } else {
        bucket = it->second;
    }

    return bucket;
}
