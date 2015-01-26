#ifndef CELL_H
#define CELL_H

#include "briq.h"

enum CellType { LIST = 1, RULE, SMBL, SPFM, FUNC, };

class Cell : public Briq {
public:
    Cell(CellType t, Baseplate *plate);
public:
    string tree();
    string to_s();
public:
    void set_ltyp(byte t);
    void set_gtyp(byte t);
    byte ltyp();
    byte gtyp();
    Briq *l();
    Briq *g();
    void set_lptr(Briq* ptr);
    void set_gptr(Briq* ptr);
    void set_lidx(briq_index idx);
    void set_gidx(briq_index idx);
public:
    void add_child(Briq *b);
protected:
    string tree_impl(const string& n);
    string to_s_impl();
    string get_type_name();
};

#endif
