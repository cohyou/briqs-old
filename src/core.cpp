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

    bool Briq::exists_in(std::string bucket_name) {
        bool result = false;
        auto it = std::find_if(signifiers.begin(), signifiers.end(),
        [=](std::shared_ptr<Sgfr> sgfr){ return (*sgfr).bucket_name() == bucket_name; });
        if (it != signifiers.end()) {
            result = true;
        }
        return result;
    }

    bool Briq::has_valid_index() {
        return (exists_in(target_signifier->bucket_name()));
    }

    void Briq::set_index(briq_index idx) {
        target_signifier->set_index(idx);
    }

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
