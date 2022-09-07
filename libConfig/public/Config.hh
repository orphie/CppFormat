#pragma once

#include <memory>   // for std::unique_ptr
class Config {
public:
    Config();
    ~Config();
public:
    int run(int argc, char* argv[]);
private:
    class ConfigImpl;
    std::unique_ptr<ConfigImpl> _pImpl;
};

