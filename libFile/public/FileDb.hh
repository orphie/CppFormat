#pragma once

#include <memory>   // for std::unique_ptr
class Config;
class FileDb {
public:
    FileDb();
    ~FileDb();
public:
    enum class DiffLineType {
        MINUS,
        PLUS,
        NOCHANGE
    };
public:
    void addFileDiffMinus(const std::string&);
    void addFileDiffPlus(const std::string&);
    void addParagDiff(size_t);
    void addLineDiff(const std::string&, DiffLineType);
private:
    class FileDbImpl;
    std::unique_ptr<FileDbImpl> _pImpl;
};

