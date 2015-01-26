#include "smbl.h"
#include "baseplate.h"
#include "stiqbucket.h"

Smbl::Smbl(Baseplate *plate)
    : Cell(SMBL, plate) {}

Smbl::Smbl(Tokn *t, Baseplate *plate)
    : Cell(SMBL, plate) {
    d.lptr = t;
}

string Smbl::tree() {
    return indent() + "SMBL[" + name() + "]";
}

string Smbl::to_s() { return name(); }

string Smbl::name() {
    string str = "";
    if (ltyp() == PNTR && d.lptr) {
        str = d.lptr->name();
    } else {
        string bucket = get_bucket();
        if (!bucket.empty() && ltyp() == INDX && d.lval != 0) {
            Briq *new_b = baseplate->load_briq(d.lval, bucket, briq_depth + 1);
            str = new_b->name();
        }
    }
    return str;
}
