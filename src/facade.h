#include "db.h"

namespace briqs {
    class Baseplate {
    public:
        template <class T, class... Types>
        T* make(Types... tpl);

        template <class T>
        Briq* save_briq(T* briq, std::string bucket_name);

        Briq* save_briq_recursive(Briq* briq, std::string bucket_name);

        // template <class T>
        Briq* load(briq_index index, std::string bucket_name);

        void clear_bucket(std::string bucket_name);

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
    Briq* Baseplate::save_briq(T* briq, std::string bucket_name) {
        auto bucket = get_bucket(bucket_name);
        return bucket->save(briq);
    }
} // namespace briqs
