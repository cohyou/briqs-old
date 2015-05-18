#include "gc.h"

namespace briqs {
    template<typename T> char *as_bytes(T& i) {
        void *addr = &i;
        return static_cast<char *>(addr);
    }

    template <class T>
    struct bucket_traits {
    };

    template <>
    struct bucket_traits<Bool> {
        byte* cast_to_data(Bool* b) {
            byte t = b->bval() ? 0xC1 : 0xC0;
            byte* data = new byte[32]{SVAL, t,};
            return data;
        }

        Bool* cast_from_data(byte *bytes) {
            return new Bool((bytes[1] == 0xC1));
        }
    };

    class Bucket {
        std::string name;
    public:
        Bucket(std::string bucket_name);

        template<class T>
        void save(T* briq);

        template<class T>
        T* load(briq_index index);

        void clear();
    private:
        void prepare();
        std::string get_file_path();
    };

    template<class T>
    void Bucket::save(T* briq)
    {
        typedef bucket_traits<T> traits;
        traits tr = traits();
        byte* data = tr.cast_to_data(briq);

        std::ofstream fout(name + ".bc", std::ios::binary | std::ios::app);
        fout.write(as_bytes(data[0]), 32);

        delete[] data;
    }

    template<class T>
    T* Bucket::load(briq_index index)
    {
        std::ifstream fin(name + ".bc", std::ios::binary);
        fin.seekg(index * 32, std::ios_base::beg);
        byte data[32] = {};
        fin.read(as_bytes(data[0]), 32);

        typedef bucket_traits<T> traits;
        traits tr = traits();
        return tr.cast_from_data(data);
    }

} // namespace briqs
