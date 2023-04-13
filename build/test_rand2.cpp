#include <random>
#include <iostream>
#include <time.h>

clock_t begin_clock;
clock_t end_clock;
double spend_time;

int main() {
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 gen(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_int_distribution<int> uni(0, 50); // guaranteed unbiased
    int i1 = 0;;
    int i2 = 0;
    int i3 = 0;
    begin_clock = clock();
    for (int i = 0; i < 100000000; i++) {
        auto random_integer2 = rand() % 50;
        i2 += random_integer2;
        //std::cout << random_integer;
    }
    end_clock = clock();
    spend_time = double(end_clock - begin_clock) / CLOCKS_PER_SEC;
    std::cout << spend_time << std::endl;

    begin_clock = clock();
    for (int i = 0; i < 100000000; i++) {
        auto random_integer = uni(gen);
        i1 += random_integer;
        //std::cout << random_integer;
    }
    end_clock = clock();
    spend_time = double(end_clock - begin_clock) / CLOCKS_PER_SEC;
    std::cout << spend_time << std::endl;

    begin_clock = clock();
    for (int i = 0; i < 100000000; i++) {
        i3 += i;
    }
    end_clock = clock();
    spend_time = double(end_clock - begin_clock) / CLOCKS_PER_SEC;
    std::cout << spend_time << std::endl;

    std::cout << i1 << i2 << i3 << std::endl;
    return 0;
}
