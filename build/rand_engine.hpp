#ifndef RAND_ENGINE_HPP

#define RAND_ENGINE_HPP

#include<random>

class RandEngine {
private:
    std::random_device rd;
    std::mt19937 generator;
    /* std::linear_congruential_engine generator; */
    std::uniform_real_distribution<> distr_float;

    float _rand(){
        return distr_float(generator);
    }

public:
    RandEngine() : 
        /* generator(rd()), */
        generator(500),
        distr_float(std::uniform_real_distribution<>(0.0,1.0)){
    }

    float operator()(){
        return _rand();
    }
};



#endif /* end of include guard RAND_ENGINE_HPP */

