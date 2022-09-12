#pragma once

#include <memory>   // for std::unique_ptr
#include <vector>

class Config;
class FileDiff;
class SrcFile;
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
public: // for diff file
    void addFileDiffMinus(const std::string&);
    void addFileDiffPlus(const std::string&);
    void addParagDiff(size_t);
    void addLineDiff(const std::string&, DiffLineType);
    std::vector<FileDiff>& getFileDiffs();
public: // for src file
    void addSrcFile(SrcFile& file);
    
private:
    class FileDbImpl;
    std::unique_ptr<FileDbImpl> _pImpl;
};

