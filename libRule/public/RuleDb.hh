#pragma once

#include <memory>   // for std::unique_ptr
class Config;
class RuleDb {
public:
    RuleDb();
    ~RuleDb();
public:
    int run(const Config&);
private:
    class RuleDbImpl;
    std::unique_ptr<RuleDbImpl> _pImpl;
};

