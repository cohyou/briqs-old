#include <sstream>
#include "spfm.h"

Spfm::Spfm(SpfmType t, Baseplate *plate)
    : Cell(SPFM, plate) {
    d.sbtp3 = t;
}

string Spfm::tree() {
    return tree_impl("SPFM[" + get_spfm_type_name() + "]");
}

string Spfm::to_s() {
    stringstream ss;

    switch (d.sbtp3) {
        case QUOT: ss << "Q"; break;
        case COND: ss << "?"; break;
        case LMBD: ss << "^"; break;
        case DEFN: ss << ":"; break;
    }

    return ss.str();
}

string Spfm::get_spfm_type_name() {
    static const string type_names[] = { "QUOT", "COND", "LMBD", "DEFN", };
    return type_names[d.sbtp3 - 9];
}
