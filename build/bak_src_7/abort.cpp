#include "abort.hpp" 

namespace inception {

void abort(std::string msg){
    std::cout<<msg<<"\n";
    std::cout<<"Quiting ...\n";
    std::terminate();
}

} // namespace inception
