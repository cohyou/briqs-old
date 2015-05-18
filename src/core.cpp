#include "core.h"

namespace briqs {
    Briq::~Briq() {}

    Bool* btob(bool b)
        { return b ? tval : fval; }

    bool is_atom(Briq* b)
        { return b->is_atom(); }

    void msg_exit(std::string msg)
    {
        std::cerr << msg << std::endl;
        exit(1);
    }

    bool Bool::bval()
        { return bval_; }

    // Sgfr::~Sgfr() {}

    std::string Cell::tree() {
        return "";
    }

} // namespace briqs
