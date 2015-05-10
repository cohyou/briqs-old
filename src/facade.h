#include "db.h"

namespace briqs {
    class Baseplate {
    public:
        template <class T, class... Types>
        T* make(Types... tpl);

        template <class T>
        void save(T* briq, std::string bucket_name);

        template <class T>
        T* load(briq_index index, std::string bucket_name);

        ~Baseplate();

    private:
        Pile pile;
        std::map<std::string, Bucket*> buckets;
        Bucket* get_bucket(std::string bucket_name);
    };

    template <class T, class... Types>
    T* Baseplate::make(Types... values)
    {
        T* g = new T(values...);
        pile.add_briq(g);
        return g;
    }

    template <class T>
    void Baseplate::save(T* briq, std::string bucket_name) {
        auto bucket = get_bucket(bucket_name);
        bucket->save(briq);
    }

    template <class T>
    T* Baseplate::load(briq_index index, std::string bucket_name) {
        auto bucket = get_bucket(bucket_name);
        auto briq = bucket->load<T>(index);
        pile.add_briq(briq);
        return briq;
    }
} // namespace briqs
