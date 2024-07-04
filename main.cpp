#include <iostream>
#include "cache.hpp"
#include <fstream>

#define BUFFER_SIZE 10

int main (int argc, const char** argv) {
    cache c(1024, 16, 2);

    if (argc != 2) {
        std::cout << "Este programa necessita de exatamente um argumento.\n";
        exit(-1);
    }

    std::ifstream file(argv[1], std::ios_base::in);
    char buffer[BUFFER_SIZE];
    uint64_t addr;

    uint64_t hit_count = 0, miss_count = 0;
    while (!file.eof()) {
        file.getline(buffer, BUFFER_SIZE);
        addr = strtoull(buffer, NULL, 10);

        if (c.access(addr)) {
            if (c.get_set_num(addr) == 1) {
                std::cout << "For addr " << addr << ", word: " << c.get_word_num(addr) << ", tag: " << c.get_tag_num(addr) << ", HIT\n";
                c.print_set(1);
            }
            hit_count++;
        } else {
            if (c.get_set_num(addr) == 1) {
                std::cout << "For addr " << addr << ", word: " << c.get_word_num(addr) << ", tag: " << c.get_tag_num(addr) << ", MISS\n";
                c.print_set(1);
            }
            miss_count++;
        }
    }
    file.close();

    std::cout << "Hit count: " << hit_count << "\nMiss count: " << miss_count << '\n';


    return 0;
}