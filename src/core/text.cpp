#include <iostream>
#include <sstream>
#include "baseplate.h"
#include "text.h"

Text::Text(Baseplate *plate)
    : Briq(VCTR, plate), texts() {
    d.type = TXT2;
}

Text::Text(string s, Baseplate *plate)
    : Briq(VCTR, plate), texts(set_textdata(s)) {
    d.type = TXT_;
}

string Text::tree() {
    stringstream ss;
    ss << indent() + "TEXT\"" + name() + "\"";
    return ss.str();
}

string Text::to_s() {
    return "\"" + name() + "\"";
}

string Text::name() {
    return vval(texts);
}

void Text::save_other_briqs() {
    for (size_t i = 0; i < texts.size(); ++i) {
        baseplate->save_briq(texts[i], get_bucket());
    }
}

void Text::load_other_briqs() {
    if (d.vctrsize > 0) {
        for (size_t i = 0; i < (d.vctrsize - 1) / sizeof(d.vctrdata); ++i) {
            Briq *t = baseplate->load_briq(get_index() + (i + 1), get_bucket(), briq_depth + 1);
            texts.push_back(t);
        }
    }
}

void Text::mark_other_briqs() {
    for (size_t i = 0; i < texts.size(); ++i) {
        texts[i]->mark();
    }
}

vector<Briq *> Text::set_textdata(string ubs) {
    vector<Briq *> vctrs;
    int l = sizeof(d.vctrdata);

    int vs = ubs.size();
    d.vctrsize = vs;

    for (int i = 0; i <= min(l, vs - 1); ++i) {
        if (i < l) {
            d.vctrdata[i] = ubs[i];
        } else {
            for (int j = 0; j < (vs - 1) / l; ++j) {
                Text *v = baseplate->make_text();
                for (int k = 0; k < l; ++k) {
                    v->vector_data()[k] = ubs[(j + 1) * l + k];
                }
                vctrs.push_back(v);
            }
        }
    }

    return vctrs;
}

string Text::vval(const vector<Briq *> vctrs) {
    stringstream ss;

    int l = sizeof(d.vctrdata);
    int vs = d.vctrsize;

    for (int i = 0; i <= min(l, vs - 1); ++i) {
        if (i < l) {
            ss << d.vctrdata[i];
        } else {
            for (int j = 0; j < (vs - 1) / l; ++j) {
                Briq *v = vctrs[j];
                for (int k = 0; k < l; ++k) {
                    if (v->vector_data()[k] == '\0' || i + k * (j + 1) == vs) {
                        return ss.str();
                    }

                    ss << v->vector_data()[k];
                }
            }
        }
    }

    return ss.str();
}
