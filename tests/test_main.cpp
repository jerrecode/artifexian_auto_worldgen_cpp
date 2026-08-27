#include "test.hpp"
int main(){int fail=0;for(auto&t:tests()){try{t.fn();std::cout<<"[PASS] "<<t.name<<'\n';}catch(const std::exception&e){++fail;std::cerr<<"[FAIL] "<<t.name<<": "<<e.what()<<'\n';}}std::cout<<tests().size()-static_cast<std::size_t>(fail)<<"/"<<tests().size()<<" tests passed\n";return fail?1:0;}
