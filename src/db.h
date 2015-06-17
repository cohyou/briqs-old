#include "gc.h"

namespace briqs {
    template<typename T> char *as_bytes(T& i) {
        void *addr = &i;
        return static_cast<char *>(addr);
    }

    class Bucket {
        std::string bucket_name;
    public:
        Bucket(std::string name);

        template<class T>
        Briq* save(T* briq);
        Briq* save_recursive(Briq* briq);

        Briq* load(briq_index index);

        void clear();
        bool operator==(Bucket another_bucket) {
            return bucket_name == another_bucket.bucket_name;
        }
        bool operator!=(Bucket another_bucket) {
            return !(*this == another_bucket);
        }
        bool operator<(Bucket another_bucket) {
            return bucket_name < another_bucket.bucket_name;
        }
        std::string name() { return bucket_name; }

    private:
        void prepare();
        std::string get_file_path();
        briq_index get_max_id();
        briq_index incr_max_id();
    };

    template<class T>
    Briq* Bucket::save(T* briq) {
        briq->set_target_bucket(this->name());

        byte* data = briq->cast_to_data();

        if (briq->has_valid_index()) {
            // 既存
            std::fstream fio(get_file_path().c_str(), std::ios::in | std::ios::out | std::ios::binary);
            fio.seekp(briq->get_index() * 32, std::ios_base::beg);
            if (briq->type() == TEXT) {
                fio.write(as_bytes(data[0]), ((briq->vstr().size() - 1) / 16 + 1) * 32);
            } else {
                fio.write(as_bytes(data[0]), 32);
            }
        } else {
            // 新規
            briq->set_index(incr_max_id());
            std::ofstream fout(get_file_path().c_str(), std::ios::binary | std::ios::app);
            if (briq->type() == TEXT) {
                int data_count = ((briq->vstr().size() - 1) / 16 + 1);
                for (int i = 0; i < data_count - 1; ++i) {
                    incr_max_id();
                }
                fout.write(as_bytes(data[0]), data_count * 32);
            } else {
                fout.write(as_bytes(data[0]), 32);
            }
        }

        delete[] data;

        return briq;
    }

    

    class Dntr : public Sgfr {
        Bucket* bucket;
        briq_index index;
    public:
        Dntr(Bucket* bc, briq_index idx) : bucket(bc), index(idx) {};
        Briq* get() override
            { return bucket->load(index); }
        SgfrType type() override
            { return SgfrType::DNTR; }
        std::string bucket_name() override
            { return bucket->name(); };
        bool operator==(Dntr another_dntr) {
            return (*bucket == *another_dntr.bucket) &&
                   (index == another_dntr.index);
        }
        bool operator<(const Dntr& another_dntr) const {
            if (*bucket != *another_dntr.bucket) {
                return *bucket < *another_dntr.bucket;
            } else {
                return index < another_dntr.index;
            }
        }
        briq_index get_index() override
            { return index; };
        void set_index(briq_index idx) override
            { index = idx; }
    };
} // namespace briqs
