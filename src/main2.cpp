#include "query.h"

using namespace briqs;

void rep(const char* path) {
    Baseplate* plate = new Baseplate();
    std::fstream fs(path);
    std::stringstream ss;
    ss << fs.rdbuf();
    std::cout << ss.str() << std::endl;
    // ss << "ab cd";

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
    
    /*
    stiq.print(stiq.eval(new Smbl("list")));
    (*stiq.eval(new Smbl("list")))(&stiq, none);
    */
}

int main() {
    /*
    Baseplate* plate = new Baseplate();
    auto c = plate->make<Cell<None, None>>(none, none);
    auto b = car(c);
    // auto d = car(b); // error

    auto t = plate->make<Text>("wowow");
    auto n = plate->make<None>();

    std::cout << t->to_s() << std::endl;

    std::string bucket_name = "bool_save_test";
    plate->save(tval, bucket_name);
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
