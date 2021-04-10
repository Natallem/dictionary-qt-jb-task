//
// Created by lnv20 on 10.04.2021.
//

#ifndef DICTIONARY_DICTIONARY_UTIL_H
#define DICTIONARY_DICTIONARY_UTIL_H

#include <string>
#include <map>
#include <unordered_set>
#include <fstream>
#include <unordered_map>
#include <vector>

namespace dictionary_util {

    std::pair<std::string, std::vector<size_t>>
    sub_string_search(std::string const &str, const std::string &pattern, const std::vector<int> &p_array);

    std::vector<int> p_array(std::string &pattern);

    /* functions for generating input dictionary format with statistic from origin dictionary source*/

    void generate_dictionary(const std::string &input_filename, const std::string &output_filename);

    bool check_generate_dictionary(
            const std::string &input_filename,
            const std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> &occurrences);

    bool check_same(const std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> &prev,
                    const std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> &new_);

    static const int margin_char_length = 10;
};


#endif //DICTIONARY_DICTIONARY_UTIL_H
