#include "monitor.h"

#include <thread>
#include <chrono>
#include <filesystem>


// constructor for filemonitor
FileMonitor::FileMonitor(const std::string &targetpath,
                         std::chrono::milliseconds interval,
                         const std::string &logfilename)
    : logger{logfilename}
    , interval{interval}
    , targetpath{targetpath}
{
    for (auto &file : std::filesystem::recursive_directory_iterator(targetpath)) {
        current_files[file.path().string()] = std::filesystem::last_write_time(file);
    }
}

// Monitor the targetpath for changes and pass information to the logger in case of differences
void FileMonitor::start(std::chrono::seconds timeout) {
    auto start = std::chrono::steady_clock::now();
    auto end = start + timeout;

    while (std::chrono::steady_clock::now() < end) {
        std::this_thread::sleep_for(interval);
        // check for state 
        // new files and changes
        for (auto &file : std::filesystem::recursive_directory_iterator(targetpath)) {
            if (current_files.find(file.path().string()) == current_files.end()) { // new file
                logger.log(file.path().string(), status::added);
                current_files[file.path().string()] = std::filesystem::last_write_time(file);
            } else if (current_files[file.path().string()] != std::filesystem::last_write_time(file)) {
                logger.log(file.path().string(), status::changed);
                current_files[file.path().string()] = std::filesystem::last_write_time(file);
            }
        }

        // removed files
        auto it = std::begin(current_files);
        while (it != std::end(current_files)) {
            if (not std::filesystem::exists(it->first)) {
                logger.log(it->first, status::removed);
                it = current_files.erase(it);
            } else {
                ++it;
            }
        }
    }
}
