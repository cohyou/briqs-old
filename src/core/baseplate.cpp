#include <iostream>
#include <algorithm>
#include "baseplate.h"

using namespace std;

None *Baseplate::make_none() {
    None *none = new None(this);
    briqs.push_back(none);
    // cout << "---- MAKE NONE ---- : " << hex << (void *)none << endl;
    return none;
}

Fval *Baseplate::make_fval() {
    Fval *fval = new Fval(this);
    briqs.push_back(fval);
    // cout << "---- MAKE FVAL ---- : " << hex << (void *)fval << endl;
    return fval;
}

Tval *Baseplate::make_tval() {
    Tval *tval = new Tval(this);
    briqs.push_back(tval);
    // cout << ""---- MAKE TVAL ---- : " << hex << (void *)tval << endl;
    return tval;
}

Ui64 *Baseplate::make_ui64(unsigned long ul) {
    Ui64 *ui64 = new Ui64(ul, this);
    briqs.push_back(ui64);
    // cout << ""---- MAKE UI64 ---- : " << hex << (void *)ui64 << endl;
    return ui64;
}

Text *Baseplate::make_text() {
    Text *text = new Text(this);
    briqs.push_back(text);
    // cout << ""---- MAKE TEXT ---- : " << hex << (void *)text << endl;
    return text;
}

Text *Baseplate::make_text(string s) {
    Text *text = new Text(s, this);
    briqs.push_back(text);
    // cout << ""---- MAKE TEXT ---- : " << hex << (void *)text << endl;
    return text;
}

List *Baseplate::make_list() {
    List *list = new List(this);
    briqs.push_back(list);
    // cout << ""---- MAKE LIST ---- : " << hex << (void *)list << endl;
    return list;
}

Briq *Baseplate::save_briq(Briq *briq, string bucket_name) {
    Bucket *bucket = make_bucket(bucket_name);
    return bucket->save_briq(briq);
}

Briq *Baseplate::save_briq_recursive(Briq *briq, string bucket_name) {
    Bucket *bucket = make_bucket(bucket_name);
    return bucket->save_briq_recursive(briq);
}

Briq *Baseplate::load_briq(const briq_index bid, string bucket_name, const int depth) {
    Bucket *bucket = make_bucket(bucket_name);
    Briq *briq = bucket->load_briq(bid, depth);
    briqs.push_back(briq);
    return briq;
}

void Baseplate::vnsh_briq(Briq *briq) {
    for (vector<Briq *>::iterator it = briqs.begin(); it != briqs.end(); ++it) {
        if (briq == *it) {
            briqs.erase(it);
            delete briq;
        }
    }
}

void Baseplate::mark_sweep_briqs(Briq *b) {
    mark_briqs(b);
    sweep_briqs();
}

bool is_not_marked(Briq *b) {
    if (!b) return false;
    return !b->is_marked();
}

void Baseplate::mark_briqs(Briq *b) {
    if (is_not_marked(b)) {
        b->mark();
        if (b->kind() == CELL) {
            if (b->ltyp() == PNTR && b->lptr()) {
                mark_briqs(b->lptr());
            }
            if (b->gtyp() == PNTR && b->gptr()) {
                mark_briqs(b->gptr());
            }
        } else if (b->type() == TXT_) {
            b->mark_other_briqs();
        }
    }
}

void Baseplate::sweep_briqs() {
    unsigned int before_size = briqs.size();
    briqs.erase(remove_if(briqs.begin(), briqs.end(), is_not_marked), briqs.end());
    // cout << "---- LIVE BRIQ ---- : " << dec << briqs.size() << endl;
    // cout << "---- DEAD BRIQ ---- : " << dec << before_size - briqs.size() << endl;
}

Bucket *Baseplate::make_bucket(string bucket_name) {
    Bucket *bucket = 0;

    map<string, Bucket *>::iterator it = buckets.find(bucket_name);
    if (it == buckets.end()) {
        bucket = new Bucket(bucket_name, this);
        buckets[bucket_name] = bucket;
    } else {
        bucket = it->second;
    }

    return bucket;
}

void Baseplate::clear_bucket(string bucket_name) {
    Bucket *bucket = make_bucket(bucket_name);
    bucket->clear();
}

unsigned int Baseplate::briq_count() const {
    return briqs.size();
}

Baseplate::~Baseplate() {
    vector<Briq *>::iterator it;
    for (it = briqs.begin(); it != briqs.end(); ++it) {
        delete *it;
    }

    map<string, Bucket *>::iterator bucket_it;
    for (bucket_it = buckets.begin(); bucket_it != buckets.end(); ++bucket_it) {
        delete bucket_it->second;
    }
}
