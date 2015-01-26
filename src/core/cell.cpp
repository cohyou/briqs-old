#include <iostream>
#include <sstream>
#include "bucket.h"
#include "cell.h"
#include "baseplate.h"

using namespace std;

Cell::Cell(CellType t, Baseplate *plate)
    : Briq(CELL, plate) {
    d.type = t;
    set_ltyp(PNTR);
    set_gtyp(PNTR);
}

string Cell::tree() {
    return tree_impl("CELL[" + get_type_name() + "]");
}

string Cell::to_s() {
    stringstream ss;
    ss << "(";
    ss << to_s_impl();
    return ss.str();
}

void Cell::set_ltyp(byte t) {
    d.sbtp1 = t;
}

void Cell::set_gtyp(byte t) {
    d.sbtp2 = t;
}

byte Cell::ltyp() {
    return d.sbtp1;
}

byte Cell::gtyp() {
    return d.sbtp2;
}

Briq *Cell::l() {
    Briq *b = 0;

    switch (ltyp()) {
        case PNTR:
        if (d.lptr) {
            b = d.lptr;
            set_lptr(d.lptr);
        }
        break;

        case INDX:
        if (d.lval != 0) {
            b = baseplate->load_briq(d.lval, get_bucket(), briq_depth + 1);
            if (b) {
                set_lptr(b);
            }
        }
        break;

        default:
        break;
    }

    return b;
}

Briq *Cell::g() {
    Briq *b = 0;

    switch (gtyp()) {
        case PNTR:
        if (d.gptr) {
            b = d.gptr;
            set_gptr(d.gptr);
        }
        break;

        case INDX:
        if (d.gval != 0) {
            b = baseplate->load_briq(d.gval, get_bucket(), briq_depth + 1);
            if (b) {
                set_gptr(b);
            }
        }
        break;

        default:
        break;
    }

    return b;
}

void Cell::set_lptr(Briq* ptr) {
    set_ltyp(PNTR);
    if (ptr) {
        ptr->set_depth(briq_depth + 1);
        string bucket = get_bucket();
        if (!bucket.empty()) {
            ptr->set_bucket(bucket);
        }
        d.lptr = ptr;
    }
}

void Cell::set_gptr(Briq* ptr)   {
    set_gtyp(PNTR);
    if (ptr) {
        ptr->set_depth(briq_depth + 1);
        string bucket = get_bucket();
        if (!bucket.empty()) {
            ptr->set_bucket(bucket);
        }
        d.gptr = ptr;
    }
}

void Cell::set_lidx(briq_index idx) {
    set_ltyp(INDX);
    d.lval = idx;
}

void Cell::set_gidx(briq_index idx) {
    set_gtyp(INDX);
    d.gval = idx;
}

void Cell::add_child(Briq *b) {
    Briq *c = l();

    if (!c) {
        d.lptr = b;
        return;
    }

    while (c) {
        if (!c->g()) {
            c->set_gptr(b);
            break;
        }
        c = c->g();
    }
}

string Cell::tree_impl(const string& n) {
    stringstream ss;

    if (n.size() > 0) {
        ss << indent() << n;
    }

    Briq *c = l();
    if (c) {
        ss << endl;
    }
    while (c) {
        c->set_depth(briq_depth + 1);
        if (c != d.lptr) {
            ss << endl;
        }
        ss << c->tree();
        c = c->g();
    }

    return ss.str();
}

string Cell::to_s_impl() {
    stringstream ss;

    if (lptr()) {
        Briq *l_b = l();
        l_b->set_depth(depth() + 1);
        if (l_b->type() == LIST) {
            ss << "(";
            ss << l_b->to_s_impl();
        } else {
            ss << l_b->to_s();
        }
    } else {
        if (gptr()) {
            ss << "N";
        }
    }

    if (gptr()) {
        Briq *g_b = g();
        if (g_b->type() == LIST) {
            ss << " ";
            ss << g_b->to_s_impl();
        } else {
            ss << " . ";
            ss << g_b->to_s();
            ss << ")";
        }
    } else {
        if (type() == LIST) {
            ss << ")";
        }
    }

    return ss.str();
}

string Cell::get_type_name() {
    static const string typeNames[] = { "INVL", "LIST", "RULE", "SMBL", "SPFM", "FUNC" };
    if (d.type >= 6) return "????";
    return typeNames[d.type];
}
