#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>
#include "briq.h"

using namespace std;

enum TextType { TXT_ = 0xD9, TXT2 = 0xDA, };

class Text : public Briq {
public:
    Text(Baseplate *plate);
    Text(string s, Baseplate *plate);
public:
    string tree();
    string to_s();
public:
    string vstr();
    string name();
public:
    void save_other_briqs();
    void load_other_briqs();
    void mark_other_briqs();
protected:
    vector<Briq *> set_textdata(string ubs);
    string vval(const vector<Briq *> vctrs);
    vector<Briq *> texts;
};

#endif
