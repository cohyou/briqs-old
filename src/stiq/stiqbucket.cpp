#include "stiqbucket.h"
#include "tokn.h"
#include "rule.h"
#include "spfm.h"
#include "func.h"
#include "smbl.h"

Stiqbucket::Stiqbucket(string bucket_name, Baseplate *plate)
    : Bucket(bucket_name, plate) {}

Briq *Stiqbucket::cast_data(byte *data, const int depth) {
    Briq *b = 0;

    switch (data[0]) {
        case VCTR:
        if (data[2] == TOKN) {
            b = new Tokn(TOKN, "", baseplate);
        }
        break;

        case CELL:
        switch (data[1]) {
            case RULE: b = new Rule("", baseplate); break;
            case SMBL: b = new Smbl(baseplate); break;
            case SPFM: b = new Spfm((SpfmType)0, baseplate); break;
            case FUNC: b = new Func((FuncType)0, baseplate); break;
            default:
            break;
        }
        break;

        default:
        break;
    }

    if (!b) {
        b = Bucket::cast_data(data, depth);
    }

    return b;
}
