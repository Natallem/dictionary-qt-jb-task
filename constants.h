#ifndef DICTIONARY_CONSTANTS_H
#define DICTIONARY_CONSTANTS_H

#include <string>

namespace constants {
    static std::string const input_dictionary_file_name = "B:\\Projects\\QtProjects\\dictionary\\words.txt";
    static std::string const processed_dictionary_file_name = "B:\\Projects\\QtProjects\\dictionary\\words_processed.txt";
    static std::string const error_open_file_message = "Cannot open words_processed.txt file for dictionary.\nPlease, add proper path to constants.h file, \'processed_dictionary_file_name\' variable\n";
    static uint64_t const INPUT_VERSION_QUIT = 0;
    static int const margin_char_length = 10; // uses in dictionary generation
}
#endif //DICTIONARY_CONSTANTS_H
