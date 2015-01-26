#include "none.h"

None::None(Baseplate *plate)
    : Sval(NONE, plate) {}

string None::to_s() {
    return "N";
}
