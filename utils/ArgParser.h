#pragma once

#include <iostream>
#include <cstring>
#include <fstream>
#include <filesystem>
#include <stdexcept>


class ArgParser {
private:
    enum class Type : int {
        input, output, maxiter, freq
    };

    static const size_t kLenInp = 8;
    static const size_t kLenOut = 9;
    static const size_t kLenIter = 9;
    static const size_t kLenFreq = 7;
    static const size_t kLenShort = 2;
    static const size_t kIntSize = 64;

public:
    struct Arguments{
        std::filesystem::path infile = "#";
        std::filesystem::path output = "./";
        uint64_t max_iter = INT64_MAX;
        uint64_t freq = 0;
    };

    Arguments arglist;

    static bool FileExists(const std::filesystem::path& filename);

    static bool isNumeric(const char* value);

    void AddIntValue(uint64_t val, const Type& type);

    void AddStrValue(std::filesystem::path& val, const Type& type);

    bool Parse(char* args[], int argc);
};



