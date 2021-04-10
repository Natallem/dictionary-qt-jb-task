//
// Created by lnv20 on 10.04.2021.
//

#include "dictionary_util.h"
#include <string>
#include <unordered_map>
#include <iomanip>
#include <vector>

std::string dictionary_util::sub_string_search(std::string &&str, const std::string &pattern,
                                               const std::vector<int> &p_array) {
    int tail = -1;
    for (int i = 0; i < str.size(); i++) {

        while (tail != -1 && str[i] != pattern[tail + 1])
            tail = p_array[tail];

        if (str[i] == pattern[tail + 1])
            tail++;

        if (tail == pattern.size() - 1){
            return std::move(str);
//            return std::move(str.insert(i+1,"</b>").insert(i-tail, "<b>"));
        }
    }
    return "";
}

std::vector<int> dictionary_util::p_array(std::string &pattern) {
    std::vector<int> result(pattern.size(), -1);
    for (int r = 1, l = -1; r < pattern.size(); r++) {
        while (l != -1 && pattern[l + 1] != pattern[r])
            l = result[l];
        if (pattern[l + 1] == pattern[r])
            result[r] = ++l;
    }
    return result;
}

void dictionary_util::generate_dictionary(const std::string &input_filename, const std::string &output_filename) {
    std::ifstream fin(input_filename, std::ios_base::binary);
    std::string cur_world;
    std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> occurrences;
    int position;
    while (fin >> cur_world) {
        std::unordered_map<char, size_t> word_chars;
        for (char ch : cur_world) {
            ++word_chars[ch];
        }
        for (auto &p : word_chars) {
            occurrences[p.first][p.second].push_back(position);
        }
        position = fin.tellg();
    }

    fin.clear();
    fin.seekg(0);
    std::ofstream fout(output_filename, std::ios_base::binary);
    position += margin_char_length;
    std::string pos_str = std::to_string(position) + '\n';
    fout << std::setw(margin_char_length) << pos_str << std::setw(0);
    while (fin >> cur_world) {
        fout << cur_world << '\n';
    }
    for (auto &pair1 : occurrences) {
        fout << pair1.first << ' ' << std::to_string(pair1.second.size()) << '\n';
        for (auto &pair2 : pair1.second) {
            fout << std::to_string(pair2.first) << ' ' << std::to_string(pair2.second.size()) << ' ';
            for (auto num : pair2.second) {
                fout << std::to_string(num + margin_char_length) << ' ';
            }
            fout << '\n';
        }
    }
    fin.close();
    fout.close();
//    if (check_generate_dictionary_util(output_filename, occurrences)){
//        std::cout << "ok!";
//    } else {
//        std::cout  << "not ok!";
//    }
}

bool dictionary_util::check_generate_dictionary(const std::string &input_filename,
                                                const std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> &occurrences) {
    std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> cur_occurrences;
    std::ifstream fin(input_filename, std::ios_base::binary);
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
                cur_occurrences[cur_char[0]][cur_char_occurrence_number].push_back(cur_position - margin_char_length);
            }
        }
    }

    return check_same(occurrences, cur_occurrences);
}

bool dictionary_util::check_same(const std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> &prev,
                                 const std::unordered_map<char, std::unordered_map<size_t, std::vector<size_t>>> &new_) {
    if (prev.size() != new_.size()) {
        return false;
    }
    return prev == new_;
}

