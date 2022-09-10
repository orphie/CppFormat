#pragma once

#include <memory>   // for std::unique_ptr
#include "libFile/public/FileDb.hh"
class Config;
class DiffFileDb {
public:
    DiffFileDb();
    ~DiffFileDb();
public:
    void addFileDiffMinus(const std::string&);
    void addFileDiffPlus(const std::string&);
    void addParagDiff(size_t);
    void addLineDiff(const std::string&, FileDb::DiffLineType);
private:
    class DiffFileDbImpl;
    std::unique_ptr<DiffFileDbImpl> _pImpl;
};

