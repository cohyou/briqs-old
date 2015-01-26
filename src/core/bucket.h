#ifndef BUCKET_H
#define BUCKET_H

#include <string>
#include "none.h"
#include "fval.h"
#include "tval.h"
#include "ui64.h"
#include "text.h"
#include "list.h"

using namespace std;

template<typename T> char *as_bytes(T& i) {
    void *addr = &i;
    return static_cast<char *>(addr);
}

class Bucket {
public:
    Bucket(string n, Baseplate *plate);
    virtual ~Bucket();
public:
    string const name();
    Briq *save_briq(Briq *b);
    Briq *save_briq_recursive(Briq *b);
    Briq *load_briq(const briq_index index, const int depth);
    void clear();
private:
    void prepare();
    briq_index get_max_id();
    briq_index incr_max_id();
    Briq *load_data(const briq_index index, const int depth);
protected:
    virtual Briq *cast_data(byte *data, const int depth);
private:
    string get_file_path();
protected:
    const string bucket_name;
    Baseplate *baseplate;
};

#endif
