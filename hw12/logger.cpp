#include "logger.h"

#include <ctime>
#include <chrono>
#include <filesystem>


// constructor, consider the already existing log file
Logger::Logger(const std::string &filename) : file{} {
    // if log already exists
    if (std::filesystem::exists(filename)) {
        if (std::filesystem::exists(filename + "_old")) {
            std::filesystem::remove(filename + "_old");
        }
        std::filesystem::rename(filename, filename + "_old");
    }
    file.open(filename);
    if (not file.is_open()) {
        throw std::runtime_error("open failed");
    }

    std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    file << "Commencing logging for directory: "
         << "TODO: insert current path here.."
         << std::filesystem::current_path()
         << std::endl
         << "Current time is: "
         << std::ctime(&time)
         << std::endl;
}

Logger::~Logger() {
    // explicit file closing needed in Windows
}

// write to the log file
void Logger::log(const std::string &path, status what) const {
    if (not file.is_open()) {
        throw std::runtime_error("File could not be opened!");
    }

    switch (what) {
    case status::added :
        file << "+ File was added:    " << path << std::endl;
        break;
    case status::changed :
        file << "! File was modified: " << path << std::endl;
        break;
    case status::removed :
        file << "~ File was deleted:  " << path << std::endl;
        break;
    default:
        file << "? Error! Unknown file status detected for: " << path << std::endl;
        break;
    }
}
