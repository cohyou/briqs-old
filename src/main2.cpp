#include "query.h"

using namespace briqs;

void rep(const char* path) {
    Baseplate* plate = new Baseplate();
    std::fstream fs(path);
    std::stringstream ss;
    ss << fs.rdbuf();
    std::cout << ss.str() << std::endl;

    /*
    Stiq stiq(ss, plate);
    log("stiq parse");
    Briq *p = stiq.parse();
    stiq.print_tree(p);
    stiq.print(p->l());
    log("");
    log("stiq evaluate");
    Briq *b = stiq.evaluate();
    log("");
    log("stiq print");
    stiq.print_tree(b);
    stiq.print(b->l());
    log("");
    */
}

int main() {
    Baseplate* plate = new Baseplate();

    std::string bucket_name = "bool_save_test";
    plate->save_briq(tval, bucket_name);
    plate->save_briq(fval, bucket_name);

    std::cout << tval->get_index() << std::endl;

    bucket_name = "ui64_save_test";
    plate->clear_bucket(bucket_name);
    auto i = plate->make<Ui64>(12345678);
    plate->save_briq(i, bucket_name);
    std::cout << i->get_index() << std::endl;

    Briq* i2 = plate->load_briq(i->get_index(), bucket_name);
    std::cout << i2->ul() << std::endl;

    bucket_name = "cell_save_test";
    plate->clear_bucket(bucket_name);
    auto ui64 = plate->make<Ui64>(87654321);
    plate->save_briq(ui64, bucket_name);
    auto cell = plate->make<Cell>();
    cell->set_lptr(ui64);
    plate->save_briq(cell, bucket_name);

    std::string path = "stiq/__.iq";
    rep(path.c_str());
}
