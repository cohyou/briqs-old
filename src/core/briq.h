#ifndef BRIQ_H
#define BRIQ_H

#include <string>
#include <map>

using namespace std;

typedef unsigned long briq_index;
typedef unsigned char byte;

enum Kind { INVL, CODE, SVAL, VCTR, CELL, };
enum LinkType { PNTR = 0xD4, INDX = 0xD5, };

class Baseplate;
class Briq {
public:
    Briq(Kind k, Baseplate *plate);
    virtual ~Briq();
public:
    size_t data_size();
    byte *data();
    Kind kind();
    byte type();
    byte sbtp3();

    unsigned long lval();
    unsigned long gval();
    Briq *lptr();
    Briq *gptr();

    byte *vector_data();

public:
    void set_bucket(string bucket_name);
    string get_bucket();
    bool has_valid_index();
    void set_index(briq_index index);
    briq_index get_index();
private:
    bool exists_in(string bucket_name);
    void set_index_of(string bucket_name, briq_index index);
    briq_index get_index_of(string bucket_name);

public:
    void mark();
    bool is_marked();
    void set_depth(unsigned int dpth);
    unsigned int depth();
protected:
    string indent();
    string get_kind_name();
public:
    string info();
    virtual string tree();
    virtual string to_s();
    virtual string to_s_impl();
public:
    virtual string name();
public:
    virtual void save_other_briqs();
    virtual void load_other_briqs();
    virtual void mark_other_briqs();
public:
    virtual byte ltyp();
    virtual byte gtyp();
    virtual Briq *l();
    virtual Briq *g();
    virtual void set_lptr(Briq* ptr);
    virtual void set_gptr(Briq* ptr);
    virtual void set_lidx(briq_index idx);
    virtual void set_gidx(briq_index idx);
public:
    virtual void add_child(Briq *b);
protected:
    Baseplate *baseplate;
    map<string, briq_index> denoters;
    string target_bucket_name;
    unsigned int briq_depth;
    union briq_data {
        byte data[32];
        struct {
            union {
                byte data1[8];
                struct {
                    byte kind;
                    byte type;
                    byte sbtp1;
                    byte sbtp2;
                    byte sbtp3;
                    byte sbtp4;
                    byte sbtp5;
                    byte sbtp6;
                };
            };
            union {
                byte data2[8];
                unsigned long vctrsize;
                unsigned long pidx;
            };
            union {
                byte vctrdata[16];
                struct {
                    union {
                        byte data3[8];
                        unsigned long lval;
                        double ldbl;
                        Briq *lptr;
                    };
                    union {
                        byte data4[8];
                        unsigned long gval;
                        double gdbl;
                        Briq *gptr;
                    };
                };
            };
        };
    };
    briq_data d;
};

#endif
