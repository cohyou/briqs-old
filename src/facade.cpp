#include "facade.h"

namespace briqs {
    Briq* Baseplate::save_briq_recursive(Briq* briq, std::string bucket_name) {
        Bucket *bucket = get_bucket(bucket_name);
        return bucket->save_recursive(briq);
    }

    // template <class T>
    Briq* Baseplate::load_briq(briq_index index, std::string bucket_name) {
        auto bucket = get_bucket(bucket_name);
        auto briq = bucket->load(index);
        pile.add_briq(briq);
        return briq;
    }

    Bucket* Baseplate::get_bucket(std::string bucket_name) {
        Bucket* bucket = nullptr;

        auto it = buckets.find(bucket_name);
        if (it == buckets.end()) {
            bucket = new Bucket(bucket_name);
            buckets[bucket_name] = bucket;
        } else {
            bucket = it->second;
        }

        return bucket;
    }

    void Baseplate::clear_bucket(std::string bucket_name) {
        Bucket *bucket = get_bucket(bucket_name);
        bucket->clear();
    }

    Baseplate::~Baseplate() {
        for (auto it = buckets.begin(); it != buckets.end(); ++it) {
            delete it->second;
        }
    }
} // namespace briqs
