#include <iostream>
#include <fstream>
#include <deque>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "libFile/public/FileAnalyzer.hh"
#include "libFile/public/FileDb.hh"
#include "libFile/public/DiffFileDb.hh"
#include "libConfig/public/ConfigDb.hh"
#include "libUtils/public/Timer.hh"
using namespace std;

void
copyFile(const std::string& curFileName, const std::string& revertFileName) {
    // create 
    std::ifstream  src(curFileName, std::ios::binary);
    std::ofstream  dst(revertFileName, std::ios::binary);
    dst << src.rdbuf();
}

std::vector<std::string> 
readFile(const std::string& curFileName)
{
    std::vector<std::string> ret;
    ifstream infile(curFileName);
    for (std::string line(""); std::getline(infile, line); ) {
        ret.push_back(line);
    }
    return ret;
}

static std::pair<size_t, size_t> 
getNumLeadingSpaceAndTab(const std::string& line) 
{
    std::pair<size_t, size_t> ret{0,0};
    for (auto c : line) {
        if (c == ' ')   ++ret.first;
        else if (c == '\t')  ++ret.second;
        else break;
    }
    return ret;
}

class FileAnalyzer::FileAnalyzerImpl {
public:
    using Base      =   FileAnalyzer;
    FileAnalyzerImpl(FileDb& file)    :   _fileDb(file) {}
public:
    void run(const ConfigDb&);
private:
    void processDiffFile(const ConfigDb&);
    void analyzeDiffFile(const std::string&);
    void analyzeSrcFilesPerFileDiff();
    void analyzeSrcFilesPerFileDiff(FileDiff&);
    //void revertPatch();
    std::vector<std::string> revertPatch(const FileDiff&);
    void revertPatch(const ParagDiff&, std::vector<std::string>&, std::vector<std::string>&, size_t&);
    void processSrcFiles(const ConfigDb&);
    void processSrcFile(const std::string&);
    void analyzeSrcFileLines(const std::vector<std::string>& lines, SrcFile& srcFile);
    void analyzeSrcFileLine(std::string line, SrcLine& srcLine);

private:
    FileDb&     _fileDb;
};

void FileAnalyzer::FileAnalyzerImpl::run(const ConfigDb& config)
{
    processDiffFile(config);
    processSrcFiles(config);
}

void FileAnalyzer::FileAnalyzerImpl::analyzeSrcFilesPerFileDiff()
{
    ScopeTimer timer("analyzeSrcFilesPerFileDiff");
    // create a new file "*.revert"
    auto& fileDiffs = _fileDb.getFileDiffs();
    for (auto& fileDiff : fileDiffs) {
        //revertPatch(fileDiff);
        analyzeSrcFilesPerFileDiff(fileDiff);
    }
}

void FileAnalyzer::FileAnalyzerImpl::revertPatch(const ParagDiff& parag, 
        std::vector<std::string>& olines, 
        std::vector<std::string>& ilines, 
        size_t& curLineNum)
{
    for (const auto& line : parag.getLines()) {
        if (line.getType() == FileDb::DiffLineType::MINUS) {
            std::string lineRecorded = line.getLineStr();
            olines.push_back(lineRecorded);
        } else {
            std::string lineInFile = ilines[curLineNum-1];
            ++curLineNum;
            std::string lineRecorded = line.getLineStr();
            assert(lineInFile == lineRecorded);
            if (line.getType() == FileDb::DiffLineType::NOCHANGE) {
                olines.push_back(lineRecorded);
            }
        }
    }
}


void FileAnalyzer::FileAnalyzerImpl::analyzeSrcFilesPerFileDiff(FileDiff& fileDiff)
{
    auto originalFileLines = revertPatch(fileDiff);
    analyzeSrcFileLines(originalFileLines, fileDiff.getSrcFile());
}

std::vector<std::string> 
FileAnalyzer::FileAnalyzerImpl::revertPatch(const FileDiff& fileDiff)
{
    const std::string& curFileName = fileDiff.getFileName();
    auto ilines = readFile(curFileName);
    std::vector<std::string> olines;

    size_t curLineNum = 1;
    for (const auto& parag : fileDiff.getParags()) {
        while (curLineNum < parag.getStartLineNum()) {
            std::string line = ilines[curLineNum-1];
            ++curLineNum;
            olines.push_back(line);
        }
        revertPatch(parag, olines, ilines, curLineNum);
    }

    for (auto line : ilines) olines.push_back(line);
    return olines;
}


//void FileAnalyzer::FileAnalyzerImpl::processSrcLine(const std::string& inputFileName)
//{
//}

void FileAnalyzer::FileAnalyzerImpl::processSrcFile(const std::string& inputFileName)
{
    std::vector<std::string> lines = readFile(inputFileName);
    SrcFile srcFile(inputFileName);
    analyzeSrcFileLines(lines, srcFile);
    //_fileDb.addSrcFile(srcFile);
}

void FileAnalyzer::FileAnalyzerImpl::analyzeSrcFileLines(const std::vector<std::string>& lines, SrcFile& srcFile)
{
    srcFile.resize(lines.size()+1);
    SrcLine root;
    srcFile[0] = root;
    std::deque<size_t> queue(1, 0);
    for (size_t lineNum = 0; lineNum < lines.size(); ++lineNum) {
        const auto& line = lines[lineNum];
        SrcLine lineInfo;
        analyzeSrcFileLine(line, lineInfo, *queue.back());
        if (isBeginScope(lineInfo) && !isEndScope(lineInfo)) {
            // new scope found
            queue.push_back(lineNum);
        }
    }
}

void FileAnalyzer::FileAnalyzerImpl::analyzeSrcFileLine(std::string line, SrcLine& srcLine)
{
    // count leading space
    auto leadingSpaceAndTab = getNumLeadingSpaceAndTab(line);
    if (leadingSpaceAndTab.first != 0 && leadingSpaceAndTab.second != 0) {
        cout << "Mix use of Space and Tab: \n\t" << line << endl;
    }
    srcLine._leadingSpace   = leadingSpaceAndTab.first;
    srcLine._leadingTab     = leadingSpaceAndTab.second;

    // check line type
    line = line.substr(leadingSpaceAndTab.first + leadingSpaceAndTab.second);

    // 1. Empty Line: Don't care rest, return
    if (line.empty()) {
        // empty line with only spaces
        srcLine._info = SrcLine::LineInfo::EMPTY;
        return;
    }

    // 2. Comment: Don't care rest, return
    if (boost::starts_with(line, "//")) {
        srcLine._info = SrcLine::LineInfo::COMMENT_SINGLE;
        return;
    }

    // 3. Pond
    if (line[0] == '#') {
        // #include/#if/...
        // For #include, format it to:
        //  <IndentAsNeed>#<NoSpace>include<OneSpace><FileName>
        // Ignore others.
        line = line.substr(1);
        auto leadingSpaceAndTab = getNumLeadingSpaceAndTab(line);
        line = line.substr(leadingSpaceAndTab.first + leadingSpaceAndTab.second);
        if (line.empty()) {
            // '#' only
            srcLine._info = srcLine._info | SrcLine::LineInfo::POND_OTHERS;
        } else if (boost::starts_with(line, "include")) {
            srcLine._info = SrcLine::LineInfo::POND_INCLUDE;
        } else {
            srcLine._info = SrcLine::LineInfo::POND_OTHERS;
        }
    }

    if (line.find("\"") != std::string::npos) {
        std::string a = "abc ";
        cout << a << endl;
    }
    /* multiple lines comment 
     */
}

void FileAnalyzer::FileAnalyzerImpl::processSrcFiles(const ConfigDb& config)
{
    if (!config.hasInputFile()) {
        return;
    }
    auto inputFileNames = config.getInputFileNames();
    for (const auto& inputFileName : inputFileNames) {
        processSrcFile(inputFileName);
    }
}

void FileAnalyzer::FileAnalyzerImpl::processDiffFile(const ConfigDb& config)
{
    if (!config.hasDiffFile()) {
        return;
    }

    // diff file specified, break to parag
    std::string diffFileName = config.getDiffFileName();
    analyzeDiffFile(diffFileName);
    analyzeSrcFilesPerFileDiff();
}

void FileAnalyzer::FileAnalyzerImpl::analyzeDiffFile(const std::string& diffFileName)
{
    using separator = boost::char_separator<char>;
    using tokenizer = boost::tokenizer<boost::char_separator<char>>;
    auto lines = readFile(diffFileName);
    for (const auto& line : lines) {
        if (boost::starts_with(line, "---")) {
            separator sep{" ", "\t"};
            tokenizer tok{line, sep};
            std::string curFileName = *(++tok.begin());
            std::cout   << "Found Orig File: " << curFileName << endl; 
            _fileDb.addFileDiffMinus(curFileName);
            continue;
        }
        if (boost::starts_with(line, "+++")) {
            separator sep{" ", "\t"};
            tokenizer tok{line, sep};
            std::string curFileName = *(++tok.begin());
            std::cout   << "Found New File: " << curFileName << endl; 
            _fileDb.addFileDiffPlus(curFileName);
            continue;
        }
        if (boost::starts_with(line, "@@")) {
            separator sep{" "};
            tokenizer tok{line, sep};
            std::string newLineNum = *(++(++tok.begin()));
            assert(newLineNum[0] == '+');
            newLineNum = newLineNum.substr(1);
            
            separator sepLineNum{","};
            tokenizer tokLineNum{newLineNum, sepLineNum};
            std::string lineNumStr = *tokLineNum.begin();
            size_t lineNum = boost::lexical_cast<size_t>(lineNumStr);
            std::cout   << "Found new Parag begin at line " << lineNum << endl; 
            _fileDb.addParagDiff(lineNum);
            continue;
        }

        if (boost::starts_with(line, "-")) {
            _fileDb.addLineDiff(line, FileDb::DiffLineType::MINUS);
            continue;
        }

        if (boost::starts_with(line, "+")) {
            _fileDb.addLineDiff(line, FileDb::DiffLineType::PLUS);
            continue;
        }

        if (boost::starts_with(line, " ")) {
            _fileDb.addLineDiff(line, FileDb::DiffLineType::NOCHANGE);
            continue;
        }
        // diff file type unrecognized 
        assert(0);
    }
}

// class FileAnalyzer
FileAnalyzer::FileAnalyzer(FileDb& file) :  _pImpl{std::make_unique<FileAnalyzerImpl>(file)}
{}

FileAnalyzer::~FileAnalyzer() = default;

void
FileAnalyzer::run(const ConfigDb& config)
{
    return _pImpl->run(config);
}
// END: class FileAnalyzer

