
#ifndef DICTIONARY_SEARCHING_RESULT_H
#define DICTIONARY_SEARCHING_RESULT_H


#include <cstdint>
#include <string>
#include <vector>

struct searching_result {
    explicit searching_result(uint64_t version);

    searching_result(std::string input, uint64_t version);

    searching_result(searching_result && other) noexcept ;

    std::string input;
    std::vector<std::string> words;
    uint64_t total_occurrences_number = 0;
    bool partial = true;
    uint64_t input_version = 0;
};

#endif //DICTIONARY_SEARCHING_RESULT_H
