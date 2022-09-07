#pragma once

#include <memory>   // for std::unique_ptr
class Config;
class Rules;
class Format {
public:
    Format();
    ~Format();
public:
    int run(const Config&, const Rules&);
private:
    class FormatImpl;
    std::unique_ptr<FormatImpl> _pImpl;
};

