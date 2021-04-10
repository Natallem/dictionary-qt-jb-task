#include <iostream>
#include <unordered_map>
#include <vector>
#include "dictionary.h"
#include "dictionary_util.h"


dictionary::dictionary(const std::string &filename) : fin(filename, std::ios_base::binary) {}

void dictionary::read_occurrences_from_file() {
    if (!occurrences.empty())
        return;
    size_t num;
    fin >> num;
    fin.seekg(num);
    std::string cur_char;
    while (fin >> cur_char) {
        std::string cur_char_occurrences;
        fin >> cur_char_occurrences;
        for (int i = 0; i < std::stoi(cur_char_occurrences); i++) {
            std::string cur_char_occurrence_number_str;
            fin >> cur_char_occurrence_number_str;
            int cur_char_occurrence_number = std::stoi(cur_char_occurrence_number_str);
            std::string words_count_str;
            fin >> words_count_str;
            int words_count = std::stoi(words_count_str);
            for (int j = 0; j < words_count; j++) {
                std::string cur_position_str;
                fin >> cur_position_str;
                int cur_position = std::stoi(cur_position_str);
                occurrences[cur_char[0]][cur_char_occurrence_number].insert(cur_position);
            }
        }
    }
}

std::unordered_set<size_t> dictionary::get_words_by_char(char ch, size_t num) {
    std::unordered_set<size_t> result;
    auto itr = occurrences[ch].lower_bound(num);
    for (; itr != occurrences[ch].end(); ++itr) {
        result.merge(std::unordered_set<size_t>(itr->second));
    }
    return result;
}

std::string  dictionary::search_sub_string(size_t pos, const std::string &input, const std::vector<int> &p_array) {
    std::string word;
    fin.clear();
    fin.seekg(pos);
    fin >> word;
    return dictionary_util::sub_string_search(std::move(word), input, p_array);
}
