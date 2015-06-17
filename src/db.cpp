#include "db.h"

namespace briqs {
    Bool* cast_to_bool(Bucket* bucket, byte *bytes) {
        return new Bool((bytes[1] == 0xC1));
    }

    Ui64* cast_to_ui64(Bucket* bucket, byte *bytes) {
        // std::cout << "cast_to_ui64" << std::endl;
        union {
            unsigned long ul;
            byte bs[8];
        } interpret_;
        for (size_t i = 0; i < 8; i++) {
            interpret_.bs[i] = bytes[i + 16];
        }
        return new Ui64(interpret_.ul);
    }

    Text* cast_to_text(Bucket* bucket, byte *bytes) {
        union {
            unsigned long ul;
            byte bs[8];
        } interpret_;
        for (size_t i = 0; i < 8; i++) {
            interpret_.bs[i] = bytes[i + 8];
        }
        unsigned long text_size = interpret_.ul;
        std::stringstream ss;
        for (int i = 0; i < text_size; ++i) {
            ss << bytes[16 * (((i + 1) / 16) + 1) + i];
        }
        for (int i = 0; i < sizeof(bytes); ++i) {
            std::cout << "i:" << i << " text:" << (int)bytes[i] << std::endl;
        }

        // ss << bytes[32 + 16 + 0];

        return new Text(ss.str());
    }

    Cell* cast_to_cell(Bucket* bucket, byte *bytes) {
        // std::cout << "cast_to_cell" << std::endl;
        union {
            unsigned long ul;
            byte bs[8];
        } interpret_;
        for (size_t i = 0; i < 8; i++) {
            interpret_.bs[i] = bytes[i + 16];
        }
        auto c = new Cell();
        if (interpret_.ul == 0) {
            c->set_lptr(none);
        } else {
            c->set_lsgr(new Dntr(bucket, interpret_.ul));
        }
        for (size_t i = 0; i < 8; i++) {
            interpret_.bs[i] = bytes[i + 24];
        }
        if (interpret_.ul == 0) {
            c->set_gptr(none);
        } else {
            c->set_gsgr(new Dntr(bucket, interpret_.ul));
        }
        return c;
    }

    std::map<int, std::function<Briq*(Bucket*, byte*)>> unpacking_traits =
    {
        {0xC0, cast_to_bool},
        {0xC1, cast_to_bool},
        {0xCF, cast_to_ui64},
        {0xD9, cast_to_text},
        {0x01, cast_to_cell},
    };

    Bucket::Bucket(std::string name) {
        bucket_name = name;
    }

    template<>
    Briq* Bucket::save(None* n) { return n; }

    Briq* Bucket::save_recursive(Briq* briq) {
        if (briq->kind() == CELL) {
            Briq *l = briq->l();
            if (l) { save_recursive(l); }
            Briq *g = briq->g();
            if (g) { save_recursive(g); }
        }

        return save(briq);
    }

    Briq* Bucket::load(briq_index index) {
        std::ifstream fin(get_file_path().c_str(), std::ios::binary);
        fin.seekg(index * 32, std::ios_base::beg);


        byte data[32] = {};
        fin.read(as_bytes(data[0]), 32);
        Briq* briq = nullptr;

        if (data[1] == 0xD9) {
            // for Text
            union {
                unsigned long ul;
                byte bs[8];
            } interpret_;
            for (size_t j = 0; j < 8; j++) {
                interpret_.bs[j] = data[8 + j];
            }

            int data_count = ((interpret_.ul - 1) / 16) + 1;
            int data_length = data_count * 32;
            std::cout << "interpret_.ul:" << interpret_.ul << std::endl;
            std::cout << "data_length:" << data_length << std::endl;
            byte data_text[data_length];

            std::ifstream fin2(get_file_path().c_str(), std::ios::binary);
            fin2.seekg(index * 32, std::ios_base::beg);
            fin2.read(as_bytes(data_text[0]), data_length);
            for (int i = 0; i < data_length; ++i) {
                std::cout << "i:" << i << " data_text:" << (int)data_text[i] << std::endl;
            }
            briq = unpacking_traits[data_text[1]](this, data_text);
        } else {
            briq = unpacking_traits[data[1]](this, data);
        }

        briq->set_target_bucket(this->name());
        briq->set_index(index);

        return briq;
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
