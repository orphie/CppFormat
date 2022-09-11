#include <vector>
#include <cassert>
#include "libFile/public/DiffFileDb.hh"

class DiffFileDb::DiffFileDbImpl {
public:
    DiffFileDbImpl()    = default;
    ~DiffFileDbImpl()   = default;
public:
    void addFileDiffMinus(const std::string& fileName)   {
        _fileDiffs.push_back(FileDiff(fileName));
    }
    void addFileDiffPlus(const std::string& fileName)   {
        _fileDiffs[_fileDiffs.size()-1].addFileDiffPlus(fileName);
    }
    void addParagDiff(size_t line)  {
        assert(!_fileDiffs.empty());
        _fileDiffs[_fileDiffs.size()-1].addParagDiff(line);
    }
    void addLineDiff(const std::string& line, FileDb::DiffLineType type) {
        _fileDiffs[_fileDiffs.size()-1].addLineDiff(line.substr(1), type);
    }
    const std::vector<FileDiff>& getFileDiffs()  const
    {
        return _fileDiffs;
    }
private:
    std::vector<FileDiff> _fileDiffs;
};


// class DiffFileDb
DiffFileDb::DiffFileDb() :  _pImpl{std::make_unique<DiffFileDbImpl>()}
{}

DiffFileDb::~DiffFileDb() = default;

void 
DiffFileDb::addFileDiffPlus(const std::string& filename)
{
    _pImpl->addFileDiffPlus(filename);
}

void 
DiffFileDb::addFileDiffMinus(const std::string& filename)
{
    _pImpl->addFileDiffMinus(filename);
}

void 
DiffFileDb::addParagDiff(size_t lineNum)
{
    _pImpl->addParagDiff(lineNum);
}

void 
DiffFileDb::addLineDiff(const std::string& line, FileDb::DiffLineType type)
{
    _pImpl->addLineDiff(line, type);
}

const std::vector<FileDiff>& 
DiffFileDb::getFileDiffs()  const
{
    return _pImpl->getFileDiffs();
}
