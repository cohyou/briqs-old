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
        return (denoter_infos.count(bucket_name) > 0);
    }

    bool Briq::has_valid_index()
        { return (exists_in(target_bucket_name)); }

    briq_index Briq::get_index_of(std::string bucket_name) {
        return denoter_infos[bucket_name];
    }

    briq_index Briq::get_index()
        { return get_index_of(target_bucket_name); }

    void Briq::set_index_of(std::string bucket_name, briq_index idx) {
        denoter_infos[bucket_name] = idx;
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

    byte* Ui64::cast_to_data() {
        union {
            unsigned long ul;
            byte bs[8];
        } interpret_;
        byte* data = new byte[32]{SVAL, 0xCF,};
        interpret_.ul = ul_;
        for (size_t i = 0; i < 8; i++) {
            data[i + 16] = interpret_.bs[i];
        }
        return data;
    }

    byte* Cell::cast_to_data() {
        byte* data = new byte[32]{CELL, 1,};
        /*
        union {
            unsigned long ul;
            byte bs[8];
        } interpret_;

        interpret_.ul = ul_;
        for (size_t i = 0; i < 8; i++) {
            data[i + 16] = interpret_.bs[i];
        }
        */
        return data;
    }

    // Sgfr::~Sgfr() {}

    std::string Cell::tree() const {
        return "";
    }

} // namespace briqs
