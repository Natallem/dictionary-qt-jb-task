#pragma once

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>
#include <optional>
#include <vector>
#include <QObject>
#include "dictionary.h"

struct searched_result {
    searched_result(uint64_t version);

    searched_result(std::string input,uint64_t version);

    searched_result(searched_result && other) noexcept ;

    std::string input;
    std::vector<std::string> words;
    uint64_t total_occurrences_number = 0;
    bool partial = true;
    uint64_t version = 0;
};

class searching_worker : public QObject {
Q_OBJECT


public:
    void set_input(std::optional<std::string> val, bool is_input_seq);

    std::atomic<uint64_t> input_version;
    std::atomic<uint64_t> output_version;

    searching_worker();

    ~searching_worker();

//    void set_input(std::optional<std::string> val);
    std::pair<searched_result, uint64_t> get_output();

signals:

    void output_changed();

private:
    void thread_process();

    void store_result(std::optional<searched_result> const &result);

private slots:

    void notify_output();

private:
    dictionary util;
    mutable std::mutex m;
    std::condition_variable input_changed;
    std::optional<std::string> input;
    bool is_seq;
    searched_result output;
    bool notify_output_queued = false;

    std::thread worker_thread;

    static uint64_t const INPUT_VERSION_QUIT = 0;

    void search_words(uint64_t last_input_version, std::optional<std::string> &val);

    void search_words(uint64_t last_input_version, std::optional<std::string> &val, bool is_cur_seq);

    void store_result(const searched_result &result);

    void add_new_word_to_output_result(uint64_t last_input_version, std::string &&word);

    void store_new_output_result(uint64_t last_input_version, const std::string &input_str);

    void complete_output_result(uint64_t last_input_version);
};