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
#include "searching_result.h"


class searching_worker : public QObject {
Q_OBJECT
public:

    std::atomic<uint64_t> output_version;

    searching_worker();

    ~searching_worker() override;

    std::tuple<searching_result, uint64_t, uint64_t> get_output();
    void set_input(std::optional<std::string> val, bool is_input_seq, uint64_t new_input_version);

    dictionary dict;
signals:

    void output_changed();


private slots:

    void notify_output();
private:
    mutable std::mutex m;
    std::condition_variable input_changed;
    std::optional<std::string> input;
    std::atomic<bool> is_seq;
    searching_result output;
    bool notify_output_queued = false;
    std::atomic<uint64_t > worker_input_version;
    std::thread worker_thread;

    void thread_process();

    void search_words(uint64_t last_input_version, std::optional<std::string> &val, bool is_cur_seq);

    void add_new_word_to_output_result(uint64_t last_input_version, std::string &&word);

    void store_new_output_result(uint64_t last_input_version, const std::string &input_str);

    void complete_output_result(uint64_t last_input_version);

    void try_to_notify_output();
};