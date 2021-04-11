
#include "searching_result.h"

searching_result::searching_result(uint64_t version) : input_version(version) {}

searching_result::searching_result(std::string input, uint64_t version) : input(std::move(input)),
                                                                          input_version(version) {}

searching_result::searching_result(searching_result &&other) noexcept: input(other.input), words(std::move(other.words)),
                                                                       partial(other.partial),
                                                                       input_version(other.input_version),
                                                                       total_occurrences_number(
                                                                            other.total_occurrences_number) {
    other.wait_to_take = false;
}