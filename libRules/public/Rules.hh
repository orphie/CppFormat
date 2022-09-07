#pragma once

#include <memory>   // for std::unique_ptr
class Config;
class Rules {
public:
    Rules();
    ~Rules();
public:
    int run(const Config&);
private:
    class RulesImpl;
    std::unique_ptr<RulesImpl> _pImpl;
};

