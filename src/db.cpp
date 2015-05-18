#include "db.h"

namespace briqs {
    Bucket::Bucket(std::string bucket_name) {
        name = bucket_name;
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
        return "buckets/" + name + ".bc";
    }

} // namespace briqs
