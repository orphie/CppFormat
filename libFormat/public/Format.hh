#pragma once

#include <memory>   // for std::unique_ptr
class ConfigDb;
class FileDb;
class RuleDb;
class Format {
public:
    Format(const ConfigDb&, const RuleDb&, const FileDb&);
    ~Format();
public:
    int run();
private:
    class FormatImpl;
    std::unique_ptr<FormatImpl> _pImpl;
};

