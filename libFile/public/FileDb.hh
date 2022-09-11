#pragma once

#include <memory>   // for std::unique_ptr
#include <vector>

class Config;
class FileDiff;
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
    const std::vector<FileDiff>& getFileDiffs() const;
private:
    class FileDbImpl;
    std::unique_ptr<FileDbImpl> _pImpl;
};

