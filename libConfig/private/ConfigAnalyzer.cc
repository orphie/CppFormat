#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include "libConfig/public/ConfigAnalyzer.hh"
#include "libConfig/public/ConfigDb.hh"
#include "libUtils/public/Timer.hh"
using namespace boost::program_options;
using namespace std;

static 
bool fileReadable(const std::string& file) 
{
    fstream fileStream;
    fileStream.open(file);
    return fileStream.good();
}

class ConfigAnalyzer::ConfigAnalyzerImpl {
public:
    using Base      =   ConfigAnalyzer;
    using Options   =   ConfigDb::Options;
    ConfigAnalyzerImpl(ConfigDb& config)    :   _options(config.getOptions()) {}
public:
    void run(int argc, char* argv[]); 
    void analyze(int argc, char* argv[]); 
    void check();

private:
    Options& _options;
};

void ConfigAnalyzer::ConfigAnalyzerImpl::run(int argc, char* argv[])
{
    analyze(argc, argv);
    check();
}

void 
ConfigAnalyzer::ConfigAnalyzerImpl::check()
{
    ScopeTimer t("ConfigDb::ConfigDbImpl::sanityCheckOptions");
    // check file exists
    std::string ret("");
    auto files = _options.count("file") ?  _options["file"].as< std::vector<std::string> >() : 
                 std::vector<std::string>();
    auto diff  =    _options.count("diff") ?
                    _options["diff"].as< std::string >() :
                    "";
    if (files.empty() && diff.empty()) {
        ret += "Error: Empty Input.\n";
    }
    if (!diff.empty() && !fileReadable(diff)) {
        ret += "Error: Diff File Does Not Exist: " + diff + "\n";
    }
    if (!files.empty()) {
        for (const auto& file : files) {
            if (!fileReadable(file)) {
                ret += "Error: Input File Does Not Exist: " + file + "\n";
            }
        }
    }
    if (!ret.empty()) {
        std::cerr << ret << std::endl;
        exit(1);
    }
}

void ConfigAnalyzer::ConfigAnalyzerImpl::analyze(int argc, char* argv[])
{
    ScopeTimer("ConfigAnalyzer::run");
    options_description desc{"Options"};
    using StrVec = std::vector<std::string>;
    desc.add_options()
        ("help,h", "Print this help")
        ("thread,t",    value<int>()->default_value(INT_MAX)
         ->notifier([](int t) {
             std::cout << "Number of threads: " << t << endl;
             }),
         "Number of threads")
        ("file,f",    value<StrVec>()
         ->notifier([](const StrVec& vec) {
             std::cout << "Input Files: \n";
             for (const auto& str : vec) std::cout << "\t" << str << "\n";
             }),
         "Input files")
        ("config,c",  value<StrVec>()
         ->notifier([](const StrVec& vec) {
             std::cout << "Input Configs: \n" ;
             for (const auto& str : vec) std::cout << "\t" << str << "\n";
             std::cout << std::endl;
             }),
         "Configs")
        ("diff,d",    value<std::string>()
         ->notifier([](const std::string& d) {
             std::cout << "Diff-File: " << d << endl;
             }),
         "Diff file")
        ("report,r",  "Report Configs");

    positional_options_description p;
    p.add("file", -1);

    store(command_line_parser(argc, argv).options(desc).positional(p).run(),
            _options);
    if (_options.count("help")) {
        cout << desc << endl;
        return;
    }
    notify(_options);
}

// class ConfigAnalyzer
ConfigAnalyzer::ConfigAnalyzer(ConfigDb& configDb) :  _pImpl{std::make_unique<ConfigAnalyzerImpl>(configDb)}
{}

ConfigAnalyzer::~ConfigAnalyzer() = default;

void
ConfigAnalyzer::run(int argc, char* argv[])
{
    return _pImpl->run(argc, argv);
}
// END: class ConfigAnalyzer

