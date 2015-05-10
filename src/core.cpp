#include "core.h"

namespace briqs {
    Briq::~Briq() {}

    const Bool* btob(bool b)
        { return b ? tval : fval; }

    bool is_atom(const Briq* b)
        { return b->is_atom(); }

    void msg_exit(std::string msg)
    {
        std::cerr << msg << std::endl;
        exit(1);
    }

    bool Bool::bval()
        { return bval_; }

    // Sgfr::~Sgfr() {}

    void Cell::tree() const {
        std::cout << to_s() << std::endl;
        std::cout << "l:";
        if (lptr) {
            lptr->tree();
        } else {
            std::cout << "なし" << std::endl;
        }
        std::cout << "g:";
        if (gptr) {
            gptr->tree();
        } else {
            std::cout << "なし" << std::endl;
        }
        std::cout << "----" << std::endl;
    }

} // namespace briqs
