#include <iostream>
#include "libFile/public/FileDb.hh"
#include "libConfig/public/ConfigDb.hh"
#include "libFile/private/DiffFileDb.hh"

using namespace std;

// class FileDbImpl
class FileDb::FileDbImpl {
public:
    FileDbImpl()    = default;
    ~FileDbImpl()   = default;
public:
    void addFileDiffMinus(const std::string& fileName) {
        _diffFileDb.addFileDiffMinus(fileName);
    }
    void addFileDiffPlus(const std::string& fileName) {
        _diffFileDb.addFileDiffPlus(fileName);
    }
    void addParagDiff(size_t line) {
        _diffFileDb.addParagDiff(line);
    }
    void addLineDiff(const std::string& line, DiffLineType type) {
        _diffFileDb.addLineDiff(line, type);
    }
private:
    DiffFileDb _diffFileDb;
};
// END: class FileDbImpl


// class FileDb
FileDb::FileDb() :  _pImpl{std::make_unique<FileDbImpl>()}
{}

FileDb::~FileDb() = default;

void 
FileDb::addFileDiffPlus(const std::string& filename)
{
    _pImpl->addFileDiffPlus(filename);
}

void 
FileDb::addFileDiffMinus(const std::string& filename)
{
    _pImpl->addFileDiffMinus(filename);
}

void 
FileDb::addParagDiff(size_t lineNum)
{
    _pImpl->addParagDiff(lineNum);
}

void 
FileDb::addLineDiff(const std::string& line, FileDb::DiffLineType type)
{
    _pImpl->addLineDiff(line, type);
}
