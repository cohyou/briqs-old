#include "gc.h"

namespace briqs {
    void Pile::add_briq(Briq* briq) {
        return briqs_.push_back(briq);
    }

    Pile::~Pile() {
        for (auto it = briqs_.begin(); it != briqs_.end(); ++it) {
            delete *it;
        }
    }
} // namespace briqs
