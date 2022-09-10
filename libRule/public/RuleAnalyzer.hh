#pragma once

#include <memory>                       // for std::unique_ptr
class RuleDb;
class FileDb;
class ConfigDb;
class RuleAnalyzer {
public:
    RuleAnalyzer(RuleDb&);
    ~RuleAnalyzer();
    void run(const ConfigDb&, const FileDb&);
private:
    class RuleAnalyzerImpl;
    std::unique_ptr<RuleAnalyzerImpl> _pImpl;
};

