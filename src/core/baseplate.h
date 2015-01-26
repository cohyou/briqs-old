#ifndef BASE_PLATE_H
#define BASE_PLATE_H

#include <vector>
#include <map>
#include "bucket.h"

using namespace std;

class Baseplate {
public:
    virtual ~Baseplate();

    None *make_none();
    Fval *make_fval();
    Tval *make_tval();
    Ui64 *make_ui64(unsigned long ul);
    Text *make_text();
    Text *make_text(string s);
    List *make_list();

    Briq *save_briq(Briq *briq, string bucket_name);
    Briq *save_briq_recursive(Briq *briq, string bucket_name);
    Briq *load_briq(const briq_index index, string bucket_name, const int depth);

    void vnsh_briq(Briq *briq);
    void mark_sweep_briqs(Briq *b);

    virtual Bucket *make_bucket(string bucket_name);
    void clear_bucket(string bucket_name);

    unsigned int briq_count() const;

protected:
    void mark_briqs(Briq *b);
    void sweep_briqs();
public:
    vector<Briq *> briqs;
    vector<Briq *> another_briqs;
protected:
    map<string, Bucket *> buckets;
};

#endif
