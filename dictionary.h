#ifndef DICTIONARY_SOURCE_H
#define DICTIONARY_SOURCE_H

#include <string>
#include <map>
#include <unordered_set>
#include <fstream>

class dictionary {
public:
    explicit dictionary(const std::string &filename);

    void read_occurrences_from_file();

    std::unordered_set<size_t> get_words_by_char(char ch, size_t num);

    std::string search_sub_string(size_t pos, const std::string &input, const std::vector<int> &p_array);

    std::string search_sub_string_seq(size_t pos, const std::string &pattern);

private:
    std::ifstream fin;
    std::unordered_map<char, std::map<size_t, std::unordered_set<size_t>>> occurrences;
};

#endif // DICTIONARY_SOURCE_H
