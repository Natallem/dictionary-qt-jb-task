#include "searching_worker.h"
#include "dictionary_util.h"
#include "constants.h"
#include <cassert>
#include <unordered_set>

searching_worker::searching_worker()
        : dict(constants::processed_dictionary_file_name),
          worker_input_version(constants::INPUT_VERSION_QUIT + 1),
          output_version(constants::INPUT_VERSION_QUIT),
          output(0),
          worker_thread([this] {
              dict.read_occurrences_from_file();
              thread_process();
          }) {}

searching_worker::~searching_worker() {
    worker_input_version = constants::INPUT_VERSION_QUIT;
    input_changed.notify_all();
    worker_thread.join();
}

void searching_worker::set_input(std::optional<std::string> val, bool is_input_seq, uint64_t new_input_version) {
    {
        std::lock_guard lg(m);
        input = std::move(val);
        is_seq = is_input_seq;
        worker_input_version = new_input_version;
    }
    input_changed.notify_all();
}

std::tuple<searching_result, uint64_t, uint64_t> searching_worker::get_output() {
    std::lock_guard lg(m);

    return {searching_result(std::move(output)), worker_input_version, output_version};
}

void searching_worker::thread_process() {
    uint64_t last_input_version = 0;
    for (;;) {
        std::optional<std::string> input_copy;
        bool is_cur_seq;
        {
            std::unique_lock lg(m);
            input_changed.wait(lg, [&] {
                return worker_input_version != last_input_version;
            });
            last_input_version = worker_input_version;
            is_cur_seq = is_seq;
            if (last_input_version == constants::INPUT_VERSION_QUIT)
                break;
            input_copy = std::move(input);
        }
        if (input_copy)
            search_words(last_input_version, input_copy, is_cur_seq);
        else
            store_new_output_result(last_input_version, "");
    }
}

void searching_worker::search_words(uint64_t last_input_version, std::optional<std::string> &val, bool is_cur_seq) {
    assert(!(*val).empty());
    std::string initial_val = *val;
    store_new_output_result(last_input_version, initial_val);
    std::unordered_map<char, size_t> letters_counter;
    std::unordered_map<char, std::unordered_set<size_t>> word_position_map;
    std::vector<size_t> to_check;
    for (auto ch : initial_val) {
        ++letters_counter[ch];
    }
    for (auto p : letters_counter) {
        word_position_map[p.first] = dict.get_words_by_char(p.first, p.second);
    }
    char picked = word_position_map.begin()->first;
    for (auto word : word_position_map[picked]) {
        bool is_ok = true;
        for (auto &p : word_position_map) {
            if (p.first == picked)
                continue;
            if (p.second.find(word) == p.second.end()) {
                is_ok = false;
                break;
            }
        }
        if (is_ok)
            to_check.push_back(word);
    }
    std::sort(to_check.begin(), to_check.end());
    if (!is_cur_seq) {
        auto p_array = dictionary_util::p_array(initial_val); // for KMP algo
        for (auto to_check_word : to_check) {
            if (last_input_version != worker_input_version)
                return;
            std::string res = dict.search_sub_string(to_check_word, initial_val, p_array);
            if (!res.empty()) {
                add_new_word_to_output_result(last_input_version, std::move(res));
            }
        }
    } else {
        for (auto to_check_word : to_check) {
            if (last_input_version != worker_input_version)
                return;
            std::string res = dict.search_sub_string_seq(to_check_word, initial_val);
            if (!res.empty()) {
                add_new_word_to_output_result(last_input_version, std::move(res));
            }
        }
    }
    complete_output_result(last_input_version);
}

void searching_worker::complete_output_result(uint64_t last_input_version) {
    std::lock_guard lg(m);
    if (output.input_version != last_input_version)
        return;
    output.partial = false;
    try_to_notify_output();
}

void searching_worker::add_new_word_to_output_result(uint64_t last_input_version, std::string &&word) {
    std::lock_guard lg(m);
    if (output.input_version != last_input_version)
        return;
    output.words.push_back(std::move(word));
    ++output.total_occurrences_number;
    try_to_notify_output();
}

void searching_worker::store_new_output_result(uint64_t last_input_version, const std::string &input_str) {
    std::lock_guard lg(m);
    output.input_version = last_input_version;
    output.input = input_str;
    output.partial = !input_str.empty();
    output.words.clear();
    output.total_occurrences_number = 0;
    try_to_notify_output();
}

void searching_worker::try_to_notify_output() {
    ++output_version;

    if (output.wait_to_take) { return; }
    else { output.wait_to_take = true; }

    if (!notify_output_queued) {
        QMetaObject::invokeMethod(this, "notify_output");
        notify_output_queued = true;
    }
}

void searching_worker::notify_output() {
    {
        std::lock_guard lg(m);
        notify_output_queued = false;
    }
    emit output_changed();
}