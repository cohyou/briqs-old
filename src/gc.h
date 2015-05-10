#include "core.h"

namespace briqs {
    class Pile {
        std::vector<Briq*> briqs_;
    public:
        void add_briq(Briq* briq);
        ~Pile();
    };
} // namespace briqs
