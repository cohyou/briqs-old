#include "list.h"

List::List(Baseplate *plate)
    : Cell(LIST, plate) {}

string List::tree() {
    return tree_impl("LIST");
}
