#include <iostream>
#include <fstream>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include "libFile/public/FileAnalyzer.hh"
#include "libFile/public/FileDb.hh"
#include "libConfig/public/ConfigDb.hh"
#include "libUtils/public/Timer.hh"
using namespace std;

class FileAnalyzer::FileAnalyzerImpl {
public:
    using Base      =   FileAnalyzer;
    FileAnalyzerImpl(FileDb& file)    :   _file(file) {}
public:
    void run(const ConfigDb&);
private:
    void revertPatch();
    void analyzeDiffFile(const ConfigDb&);
    void analyzeSrcFiles(const ConfigDb&);
private:
    FileDb&     _file;
};

void FileAnalyzer::FileAnalyzerImpl::run(const ConfigDb& config)
{
    analyzeDiffFile(config);
    revertPatch();

    analyzeSrcFiles(config);
}

void FileAnalyzer::FileAnalyzerImpl::revertPatch()
{}

void FileAnalyzer::FileAnalyzerImpl::analyzeSrcFiles(const ConfigDb& config)
{}


void FileAnalyzer::FileAnalyzerImpl::analyzeDiffFile(const ConfigDb& config)
{
    if (!config.hasDiffFile()) {
        return;
    }

    // diff file specified, break to parag
    std::string diffFileName = config.getDiffFileName();
    ifstream infile(diffFileName);

    using separator = boost::char_separator<char>;
    using tokenizer = boost::tokenizer<boost::char_separator<char>>;
    for (std::string line(""); std::getline(infile, line); ) {
        if (boost::starts_with(line, "---")) {
            separator sep{" ", "\t"};
            tokenizer tok{line, sep};
            std::string curFileName = *(++tok.begin());
            std::cout   << "Found Orig File: " << curFileName << endl; 
            _file.addFileDiffMinus(curFileName);
            continue;
        }
        if (boost::starts_with(line, "+++")) {
            separator sep{" ", "\t"};
            tokenizer tok{line, sep};
            std::string curFileName = *(++tok.begin());
            std::cout   << "Found New File: " << curFileName << endl; 
            _file.addFileDiffPlus(curFileName);
            continue;
        }
        if (boost::starts_with(line, "@@")) {
            separator sep{" "};
            tokenizer tok{line, sep};
            std::string newLineNum = *(++tok.begin());
            assert(newLineNum[0] == '-');
            newLineNum = newLineNum.substr(1);
            
            separator sepLineNum{","};
            tokenizer tokLineNum{newLineNum, sepLineNum};
            std::string lineNumStr = *tokLineNum.begin();
            size_t lineNum = boost::lexical_cast<size_t>(lineNumStr);
            std::cout   << "Found new Parag begin at line " << lineNum << endl; 
            _file.addParagDiff(lineNum);
            continue;
        }

        if (boost::starts_with(line, "-")) {
            _file.addLineDiff(line, FileDb::DiffLineType::MINUS);
            continue;
        }

        if (boost::starts_with(line, "+")) {
            _file.addLineDiff(line, FileDb::DiffLineType::PLUS);
            continue;
        }

        if (boost::starts_with(line, " ")) {
            _file.addLineDiff(line, FileDb::DiffLineType::NOCHANGE);
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

