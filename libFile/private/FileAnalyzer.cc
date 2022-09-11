#include <iostream>
#include <fstream>
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
    void revertPatch();
    void revertPatch(const FileDiff&);
    void revertPatch(const ParagDiff&, ofstream& ofile, ifstream&, size_t&);
    void processSrcFiles(const ConfigDb&);
    void processSrcFile(const std::string&);
private:
    FileDb&     _fileDb;
};

void FileAnalyzer::FileAnalyzerImpl::run(const ConfigDb& config)
{
    processDiffFile(config);
    processSrcFiles(config);
}

void FileAnalyzer::FileAnalyzerImpl::revertPatch()
{
    ScopeTimer timer("Revert Patch");
    // create a new file "*.revert"
    const auto& fileDiffs = _fileDb.getFileDiffs();
    for (const auto& fileDiff : fileDiffs) {
        revertPatch(fileDiff);
    }
}

void FileAnalyzer::FileAnalyzerImpl::revertPatch(const ParagDiff& parag, ofstream& ofile, ifstream& ifile, size_t& curLineNum)
{
    for (const auto line : parag.getLines()) {
        if (line.getType() == FileDb::DiffLineType::MINUS) {
            std::string lineRecorded = line.getLineStr();
            ofile << lineRecorded << "\n";
        } else {
            ++curLineNum;
            std::string lineInFile("");
            std::getline(ifile, lineInFile);
            std::string lineRecorded = line.getLineStr();
            assert(lineInFile == lineRecorded);
            if (line.getType() == FileDb::DiffLineType::NOCHANGE) {
                ofile << lineInFile << "\n";
            }
        }
    }
}

void FileAnalyzer::FileAnalyzerImpl::revertPatch(const FileDiff& fileDiff)
{
    const std::string& curFileName = fileDiff.getFileName();
    std::string revertFileName = curFileName + ".revert";

    std::ifstream  ifile(curFileName, std::ios::binary);
    std::ofstream  ofile(revertFileName, std::ios::binary);

    size_t curLineNum = 1;
    for (const auto& parag : fileDiff.getParags()) {
        while (curLineNum < parag.getStartLineNum()) {
            std::string line("");
            std::getline(ifile, line);
            ofile << line << "\n";
            ++curLineNum;
        }
        revertPatch(parag, ofile, ifile, curLineNum);
    }

    for (std::string line(""); std::getline(ifile, line); ) {
        ofile << line << "\n";
        ++curLineNum;
    }
}


//void FileAnalyzer::FileAnalyzerImpl::processSrcLine(const std::string& inputFileName)
//{
//}

void FileAnalyzer::FileAnalyzerImpl::processSrcFile(const std::string& inputFileName)
{
    ifstream infile(inputFileName);
    size_t lineNum = 0;
    //InputFile src(diffFileName);
    for (std::string line(""); std::getline(infile, line); ) {
        //processSrcLine(src, line, lineNum);
        ++lineNum;
        if (line.empty()) {
            continue;
        }

        auto leadingSpaceAndTab = getNumLeadingSpaceAndTab(line);
        std::cout   << "Line " << lineNum << ": " << leadingSpaceAndTab.first 
                    << " " << leadingSpaceAndTab.second << endl;
        if (leadingSpaceAndTab.first != 0 && leadingSpaceAndTab.second != 0) {
            cout << "Mix use of Space and Tab: \n\t" << line << endl;
        }
        //if 
    }
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
    revertPatch();
}

void FileAnalyzer::FileAnalyzerImpl::analyzeDiffFile(const std::string& diffFileName)
{
    ifstream infile(diffFileName);
    using separator = boost::char_separator<char>;
    using tokenizer = boost::tokenizer<boost::char_separator<char>>;
    for (std::string line(""); std::getline(infile, line); ) {
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

