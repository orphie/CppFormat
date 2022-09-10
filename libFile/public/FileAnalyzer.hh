#pragma once

#include <memory>                       // for std::unique_ptr
class FileDb;
class ConfigDb;
class FileAnalyzer {
public:
    FileAnalyzer(FileDb&);
    ~FileAnalyzer();
    void run(const ConfigDb&);
private:
    class FileAnalyzerImpl;
    std::unique_ptr<FileAnalyzerImpl> _pImpl;
};

