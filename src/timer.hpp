#ifndef TIMER_HPP

#define TIMER_HPP

#include <iostream>
#include <chrono>
#define span std::chrono::duration_cast<std::chrono::duration<double>>


class Timer {
public:
    Timer(){
        _start =std::chrono::high_resolution_clock::now();
    }

    void reset(){
        _start =std::chrono::high_resolution_clock::now();
    }

    void print_elapsed_time(){
        _end = std::chrono::high_resolution_clock::now();
        auto time_span = span(_end - _start);
        std::cout<<"\nTime elapsed: "<<time_span.count() << " seconds.\n";
    }

    private:
        std::chrono::high_resolution_clock::time_point _start;
        std::chrono::high_resolution_clock::time_point _end;
};

#endif /* end of include guard TIMER_HPP */

