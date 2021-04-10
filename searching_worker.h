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

struct searched_result
{
    searched_result(std::string input, std::vector<std::string> factors, bool partial);

    std::string input;
    std::vector<std::string> words;
    bool partial;
};

class searching_worker : public QObject
{
Q_OBJECT

public:
    searching_worker();
    ~searching_worker();

    void set_input(std::optional<std::string> val);
    std::optional<searched_result> get_output() const;

signals:
    void output_changed();

private:
    void thread_process();
    void store_result(std::optional<searched_result> const& result);

private slots:
    void notify_output();

private:
    dictionary util;
    mutable std::mutex m;
    std::atomic<uint64_t> input_version;
    std::condition_variable input_changed;
    std::optional<std::string> input;
    std::optional<searched_result> output;
    bool notify_output_queued = false;

    std::thread worker_thread;

    static uint64_t const INPUT_VERSION_QUIT = 0;

    void search_words(uint64_t last_input_version, std::optional<std::string> &val);
};