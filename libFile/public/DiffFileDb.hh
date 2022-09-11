#pragma once
#include <memory>   // for std::unique_ptr
#include <string> 
#include <vector> 
#include "libFile/public/FileDb.hh"

class LineDiff {
public:
    LineDiff(const std::string& lineStr, size_t lineNum, FileDb::DiffLineType type) :
        _lineStr(lineStr), 
        _lineNum(lineNum),
        _type(type) {}
    const std::string& getLineStr()         const   {return _lineStr;}
    const FileDb::DiffLineType getType()    const   {return _type;}
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
    const LineDiffs& getLines() const {return _lines;}
    size_t  getStartLineNum()   const   {return _startLineNum;}
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

    const std::string& getFileName()  const {return _newFileName;}
    const ParagDiffs   getParags()    const {return _parags;}
private:
    std::string _origFileName;
    std::string _newFileName;
    ParagDiffs  _parags;
};

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
    const std::vector<FileDiff>& getFileDiffs()  const;
private:
    class DiffFileDbImpl;
    std::unique_ptr<DiffFileDbImpl> _pImpl;
};

