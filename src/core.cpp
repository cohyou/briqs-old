#include "core.h"

namespace briqs {
    Briq::~Briq() {}

    Bool* btob(bool b)
        { return b ? tval : fval; }

    bool is_atom(Briq* b)
        { return b->is_atom(); }

    void msg_exit(std::string msg)
    {
        std::cerr << msg << std::endl;
        exit(1);
    }

    void Briq::set_target_bucket(std::string bucket_name)
        { target_bucket_name = bucket_name; }

    bool Briq::exists_in(std::string bucket_name) {
        bool result = false;
        auto it = std::find_if(signifiers.begin(), signifiers.end(),
        [=](Sgfr* sgfr){ return (*sgfr).bucket_name() == bucket_name; });
        return it != signifiers.end();
    }

    bool Briq::has_valid_index()
        { return (exists_in(target_bucket_name)); }

    briq_index Briq::get_index_of(string bucket_name) {
        auto it = std::find_if(signifiers.begin(), signifiers.end(),
        [=](Sgfr* sgfr){ return (*sgfr).bucket_name() == bucket_name; });
        if (it != signifiers.end()) {
            return (*it)->get_index();
        } else {
            return ULONG_MAX;
        }
    }

    briq_index Briq::get_index() const
        { return get_index_of(target_bucket_name); }

    void Briq::set_index_of(string bucket_name, briq_index idx) {
        auto it = std::find_if(signifiers.begin(), signifiers.end(),
        [=](Sgfr* sgfr){ return (*sgfr).bucket_name() == bucket_name; });
        if (it != signifiers.end()) {
            return (*it)->set_index(idx);
        } else {
            return signifiers.insert();
        }
    }

    void Briq::set_index(briq_index idx)
        { set_index_of(target_bucket_name, idx); }

    bool Bool::bval()
        { return bval_; }

    byte* Bool::cast_to_data() {
        byte t = bval_ ? 0xC1 : 0xC0;
        byte* data = new byte[32]{SVAL, t,};
        return data;
    }

    // Sgfr::~Sgfr() {}

    std::string Cell::tree() const {
        return "";
    }

} // namespace briqs
