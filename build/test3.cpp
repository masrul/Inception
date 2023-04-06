#include <iostream>
#include <vector>

class Entity {
public: 
    Entity(size_t n){
        data = new int[n];
    }
    int* data; 

    ~Entity(){
        std::cout<<"destructor..\n";
        /* if (data !=nullptr){ */
        /*     delete [] data; */        
        /* } */
    }
};


int main()
{
    std::vector<Entity> collection;

    for (auto i=1u;i <=2 ;++i){
        collection.emplace_back(i);
    }
    
    return 0;
}

