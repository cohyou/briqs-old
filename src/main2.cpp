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

    /*
    auto t = plate->make<Text>("wowow");

    plate->save(fval, bucket_name);

    auto load_0 = plate->load<Bool>(0, bucket_name);
    std::cout << load_0->to_s() << std::endl;
    std::cout << load_0->bval() << std::endl;
    auto load_1 = plate->load<Bool>(1, bucket_name);
    std::cout << load_1->to_s() << std::endl;
    std::cout << load_1->bval() << std::endl;
    */

    std::string path = "stiq/__.iq";
    rep(path.c_str());
}
