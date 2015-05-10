#include "facade.h"

namespace briqs {
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

    Baseplate::~Baseplate() {
        for (auto it = buckets.begin(); it != buckets.end(); ++it) {
            delete it->second;
        }
    }
} // namespace briqs
