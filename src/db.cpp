#include "db.h"

namespace briqs {
    Bool* cast_from_bool(byte *bytes) {
        return new Bool((bytes[1] == 0xC1));
    }

    std::map<int, std::function<Briq*(byte*)>> unpacking_traits =
    {
        {0xC0, cast_from_bool},
        {0xC1, cast_from_bool},
    };

    Bucket::Bucket(std::string name) {
        bucket_name = name;
    }

    Briq* Bucket::save_recursive(Briq* briq) {
        // 以下、既存の処理
        /*
        b->set_bucket(this->name());

        if (b->kind() == CELL) {
            if (b->ltyp() == PNTR && b->lptr()) {
                save_briq_recursive(b->lptr());
                b->set_lidx(b->lptr()->get_index());
            } else if (b->ltyp() == INDX && b->lval() != 0) {
                Briq *new_b = load_briq(b->lval(), b->depth() + 1);
                save_briq_recursive(new_b);
                b->set_lidx(new_b->get_index());
            }

            if (b->gtyp() == PNTR && b->gptr()) {
                save_briq_recursive(b->gptr());
                b->set_gidx(b->gptr()->get_index());
            } else if (b->gtyp() == INDX && b->gval() != 0) {
                Briq *new_b = load_briq(b->gval(), b->depth() + 1);
                save_briq_recursive(new_b);
                b->set_gidx(new_b->get_index());
            }
        }

        return save_briq(b);
        */

        return briq;
    }

    Briq* Bucket::load(briq_index index) {
        std::ifstream fin(bucket_name + ".bc", std::ios::binary);
        fin.seekg(index * 32, std::ios_base::beg);
        byte data[32] = {};
        fin.read(as_bytes(data[0]), 32);
        /*
        typedef bucket_traits<T> traits;
        traits tr = traits();
        return tr.cast_from_data(data);
        */
        return unpacking_traits[data[1]](data);
    }

    void Bucket::clear() {
        std::ifstream fin(get_file_path().c_str());
        if (fin.is_open()) remove(get_file_path().c_str());
        fin.close();

        prepare();
    }

    void Bucket::prepare() {
        std::ifstream fin(get_file_path().c_str());
        if (!fin.is_open()) {
            std::ofstream fout(get_file_path().c_str(), std::ios::binary | std::ios::app);
            char cs[32] = {};
            fout.write(cs, 32);
        }
    }

    std::string Bucket::get_file_path() {
        return "buckets/" + bucket_name + ".bc";
    }

    briq_index Bucket::get_max_id() {
        briq_index mxid;

        std::fstream fio(get_file_path().c_str(), std::ios::in | std::ios::out | std::ios::binary);
        fio.seekp(24, std::ios_base::beg);
        fio.read(as_bytes(mxid), sizeof(mxid));

        return mxid;
    }

    briq_index Bucket::incr_max_id() {
        briq_index newmxid = get_max_id() + 1;

        std::fstream fio(get_file_path().c_str(), std::ios::in | std::ios::out | std::ios::binary);
        fio.seekp(24, std::ios_base::beg);
        fio.write(as_bytes(newmxid), sizeof(newmxid));

        return newmxid;
    }

} // namespace briqs
