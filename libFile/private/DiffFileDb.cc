#include <vector>
#include <cassert>
#include "libFile/public/FileDb.hh"
#include "libFile/private/DiffFileDb.hh"

class LineDiff {
public:
    LineDiff(const std::string& lineStr, size_t lineNum, FileDb::DiffLineType type) :
        _lineStr(lineStr), 
        _lineNum(lineNum),
        _type(type) {}
private:
    std::string _lineStr;
    size_t      _lineNum;
    FileDb::DiffLineType    _type;
};
using LineDiffs = std::vector<LineDiff>;

class ParagDiff {
public:
    ParagDiff(size_t line)  :   _startLineNum(line) {}
    void addLineDiff (const std::string& line, FileDb::DiffLineType type) { 
        if (_lines.empty()) {
            // first line
            assert(type == FileDb::DiffLineType::NOCHANGE);
            auto lineNum = _startLineNum;
            _curLineNumNoChange = _startLineNum + 1;
            _curLineNumMinus    = _startLineNum + 1;
            _curLineNumPlus     = _startLineNum + 1;
            _lines.push_back(LineDiff(line, lineNum, type)); 
            return;
        } else {
            if (type == FileDb::DiffLineType::NOCHANGE) {
                _lines.push_back(LineDiff(line, _curLineNumNoChange++, type)); 
                ++_curLineNumMinus;
                ++_curLineNumPlus;
            } else if (type == FileDb::DiffLineType::PLUS) {
                _lines.push_back(LineDiff(line, _curLineNumPlus++, type)); 
                ++_curLineNumNoChange;
                ++_curLineNumMinus;
            } else {
                assert(type == FileDb::DiffLineType::MINUS);
                _lines.push_back(LineDiff(line, _curLineNumMinus++, type)); 
            }
        }
    }
private:
    size_t      _startLineNum;
    size_t      _curLineNumNoChange;
    size_t      _curLineNumPlus;
    size_t      _curLineNumMinus;
    LineDiffs   _lines;
};
using ParagDiffs = std::vector<ParagDiff>;

class FileDiff {
public:
    FileDiff(const std::string& fileName)   :   _origFileName(fileName) {}
    void addFileDiffPlus(const std::string& fileName)   { _newFileName = fileName; }
    void addParagDiff(size_t line)                      { _parags.push_back(ParagDiff(line)); }
    void addLineDiff (const std::string& line, FileDb::DiffLineType type) { _parags[_parags.size() - 1].addLineDiff(line, type); }
private:
    std::string _origFileName;
    std::string _newFileName;
    ParagDiffs  _parags;
};

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
        _fileDiffs[_fileDiffs.size()-1].addLineDiff(line, type);
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
