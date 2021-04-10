#include "searching_worker.h"
#include "dictionary_util.h"
#include <cassert>
#include <unordered_set>

searched_result::searched_result(std::string input, std::vector<std::string> factors, bool incomplete)
        : input(std::move(input)), words(std::move(factors)), partial(incomplete) {}

searching_worker::searching_worker()
        : util("..\\dic_2.txt"),
          input_version(INPUT_VERSION_QUIT + 1),
          worker_thread([this] {
              util.read_occurrences_from_file();
              thread_process();
          }) {}

searching_worker::~searching_worker() {
    input_version = INPUT_VERSION_QUIT;
    input_changed.notify_all();
    worker_thread.join();
}

void searching_worker::set_input(std::optional<std::string> val) {
    {
        std::lock_guard lg(m);
        input = std::move(val);
        ++input_version;
    }
    input_changed.notify_all();
}

std::optional<searched_result> searching_worker::get_output() const {
    std::lock_guard lg(m);
    return output;
}

void searching_worker::thread_process() {
    uint64_t last_input_version = 0;
    for (;;) {
        std::optional<std::string> input_copy;
        {
            std::unique_lock lg(m);
            input_changed.wait(lg, [&] {
                return input_version != last_input_version;
            });
            last_input_version = input_version;
            if (last_input_version == INPUT_VERSION_QUIT)
                break;
            input_copy = std::move(input);
        }
        std::optional<searched_result> result;
        if (input_copy)
            search_words(last_input_version, input_copy);
        else
            store_result(std::nullopt);
    }
}

void searching_worker::search_words(uint64_t last_input_version, std::optional<std::string> &val) {
    assert(!(*val).empty());
    std::string initial_val = *val;
    std::vector<std::string> picked_words;
    store_result(searched_result(*val, picked_words, true));
    std::unordered_map<char, size_t> letters_counter;
    std::unordered_map<char, std::unordered_set<size_t>> word_position_map;
    std::vector<size_t> to_check;
    for (auto ch : initial_val) {
        ++letters_counter[ch];
    }
    for (auto p : letters_counter) {
        word_position_map[p.first] = util.get_words_by_char(p.first, p.second);
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
    auto p_array = dictionary_util::p_array(initial_val);
    for (auto to_check_word : to_check) {
        if (last_input_version != input_version)
            return;
        std::string res = util.search_sub_string(to_check_word, initial_val, p_array);
        if (!res.empty()) {
            picked_words.push_back(res);
            store_result(searched_result(initial_val, picked_words, true));
        }
    }
    store_result(searched_result(initial_val, picked_words, false));
}

void searching_worker::store_result(std::optional<searched_result> const &result) {
    std::lock_guard lg(m);
    output = result;

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
