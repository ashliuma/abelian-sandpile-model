#include "ArgParser.h"


bool ArgParser::FileExists(const std::filesystem::path& filename) {
    std::ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) {
        std::cerr << "No such file or directory";
        exit(1);
    }
    return true;
}

bool ArgParser::isNumeric(const char* value) {
    size_t i = 0;
    while(value[i] != '\0'){
        if (!isdigit(value[i])) {
            return false;
        }
        i++;
    }
    return true;
}

void ArgParser::AddIntValue(uint64_t val, const ArgParser::Type& type){
    if (type == ArgParser::Type::maxiter){
        arglist.max_iter = val;
    }
    if (type == ArgParser::Type::freq){
        arglist.freq = val;
    }
}

void ArgParser::AddStrValue(std::filesystem::path& val, const ArgParser::Type& type){
    if (type == ArgParser::Type::input){
        arglist.infile = val;
    }
    if (type == ArgParser::Type::output){
        arglist.output = val;
    }
}

bool ArgParser::Parse(char* args[], const int argc) {
    size_t i = 1;
    while (i < argc) {

        bool isCorrectArgument = false;
        if (args[i][0] == '-') {
            if (args[i][1] == '-') {
                size_t len = strlen(args[i]);

                if (len >= kLenInp && std::strncmp(args[i], "--input=", kLenInp) == 0){
                    char *end = nullptr;
                    std::filesystem::path filepath;
                    for (size_t j = kLenInp; j < len; ++j){
                        filepath += args[i][j];
                    }
                    if (end != args[i] + kLenInp) {
                        std::cerr << "No such argument was given : --input";
                        exit(1);
                    }
                    if (errno != ERANGE){
                        if (ArgParser::FileExists(filepath)){
                            ArgParser::AddStrValue(filepath, ArgParser::Type::input);
                            isCorrectArgument = true;
                        }
                        else{
                            std::cerr << "No such file or directory";
                            exit(1);
                        }
                    }
                    else{
                        std::cerr << "No such argument was given : --input";
                        exit(1);
                    }
                }

                if (len >= kLenOut && std::strncmp(args[i], "--output=", kLenOut) == 0){
                    char *end = nullptr;
                    std::filesystem::path filepath;
                    for (size_t j = kLenOut; j < len; ++j){
                        filepath += args[i][j];
                    }
                    if (end != args[i] + kLenOut){
                        std::cerr << "No such argument was given : --output";
                        exit(1);
                    }
                    if (errno != ERANGE){
                        ArgParser::AddStrValue(filepath, ArgParser::Type::output);
                        isCorrectArgument = true;
                    }
                    else{
                        std::cerr << "No such argument was given : --output";
                        exit(1);
                    }
                }

                if (len >= kLenIter && std::strncmp(args[i], "--max-iter=", kLenIter) == 0){
                    char* end;
                    char number[kIntSize];
                    size_t d = 0;
                    for (size_t j = kLenIter; j < len; ++j){
                        number[d] = args[i][j];
                        d++;
                    }
                    number[d] = '\0';
                    if (!isNumeric(number)) {
                        std::cerr << "The value is not an integer, but the parameter is only for integers";
                        exit(1);
                    }
                    uint64_t value = std::strtoll(args[i], &end, 10);
                    if (end != args[i] + kLenIter){
                        std::cerr << "No such argument was given : --max-iter";
                        exit(1);
                    }
                    if (errno != ERANGE){
                        AddIntValue(value, ArgParser::Type::maxiter);
                        isCorrectArgument = true;
                    }
                    else{
                        std::cerr << "No such argument was given : --max-iter";
                        exit(1);
                    }
                }

                if (len >= kLenFreq && std::strncmp(args[i], "--freq=", kLenFreq) == 0){
                    char* end;
                    char number[64];
                    size_t d = 0;
                    for (size_t j = kLenFreq; j < len; ++j){
                        number[d] = args[i][j];
                        d++;
                    }
                    number[d] = '\0';
                    if (!isNumeric(number)) {
                        std::cerr << "The value is not an integer, but the parameter is only for integers";
                        exit(1);
                    }
                    uint64_t value = std::strtoll(args[i], &end, 10);
                    if (end != args[i] + kLenFreq) {
                        std::cerr << "No such argument was given : --max-iter";
                        exit(1);
                    }
                    if (errno != ERANGE){
                        AddIntValue(value, ArgParser::Type::freq);
                        isCorrectArgument = true;
                    }
                    else{
                        std::cerr << "No such argument was given : --max-iter";
                        exit(1);
                    }
                }
            }
            else{

                if(i + 1 < argc){
                    size_t len = strlen(args[i + 1]);
                    if (std::strncmp(args[i], "-i", kLenShort) == 0){
                        std::filesystem::path filepath;
                        for (size_t j = 0; j < len; ++j){
                            filepath += args[i + 1][j];
                        }
                        if (ArgParser::FileExists(filepath)){
                            AddStrValue(filepath, ArgParser::Type::input);
                            isCorrectArgument = true;
                        }
                        else{
                            std::cerr << "No such file or directory";
                            exit(1);
                        }
                    }

                    if (std::strncmp(args[i], "-o", kLenShort) == 0){
                        std::filesystem::path filepath;
                        for (size_t j = 0; j < len; ++j){
                            filepath += args[i + 1][j];
                        }
                        AddStrValue(filepath, ArgParser::Type::output);
                        isCorrectArgument = true;
                    }

                    if (std::strncmp(args[i], "-m", kLenShort) == 0){
                        char number[64];
                        size_t d = 0;
                        for (size_t j = kLenFreq; j < len; ++j){
                            number[d] = args[i + 1][j];
                            d++;
                        }
                        number[d] = '\0';
                        if (!isNumeric(number)) {
                            std::cerr << "The value is not an integer, but the parameter is only for integers";
                            exit(1);
                        }
                        char* end;
                        uint64_t value = std::strtoll(args[i + 1], &end, 10);
                        AddIntValue(value, ArgParser::Type::maxiter);
                        isCorrectArgument = true;
                    }

                    if (std::strncmp(args[i], "-f", kLenShort) == 0){
                        char number[64];
                        size_t d = 0;
                        for (size_t j = 0; j < len; ++j){
                            number[d] = args[i + 1][j];
                            d++;
                        }
                        number[d] = '\0';
                        if (!isNumeric(number)) {
                            std::cerr << "The value is not an integer, but the parameter is only for integers";
                            exit(1);
                        }
                        char* end;
                        uint64_t value = std::strtoll(args[i + 1], &end, 10);
                        AddIntValue(value, ArgParser::Type::freq);
                        isCorrectArgument = true;
                    }

                    if (!isCorrectArgument){
                        std::cerr << "Incorrect arguments";
                        exit(1);
                    }
                }
                else {
                    std::cerr << "Not enough arguments";
                    exit(1);
                }
                i++;
            }
        }
        i++;
    }

    return true;
}
