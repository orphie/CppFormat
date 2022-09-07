#include <iostream>
#include <fstream>
#include "libConfig/public/Config.hh"
#include "libConfig/private/OptionParser.hh"

using namespace std;
using namespace boost::program_options;

static 
bool fileReadable(const std::string& file) 
{
    fstream fileStream;
    fileStream.open(file);
    return fileStream.good();
}

// class ConfigImpl
class Config::ConfigImpl {
public:
    ConfigImpl()    = default;
    ~ConfigImpl()   = default;
public:
    int run(int argc, char* argv[]);
private:
    std::string sanityCheckOptions()  const;
private:
    using Options = OptionParser::Options;
    Options _options;
};

int 
Config::ConfigImpl::run(int argc, char* argv[]) {
    // analyze options
    OptionParser(_options).run(argc, argv);

    // sanity check on options
    auto err = sanityCheckOptions();
    if (!err.empty()) {
        std::cerr << err << std::endl;
        return 1;
    }

    return 0;
}

std::string 
Config::ConfigImpl::sanityCheckOptions()    const
{
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
    return ret;
}

// END: class ConfigImpl


// class Config
Config::Config() :  _pImpl{std::make_unique<ConfigImpl>()}
{}

Config::~Config() = default;

int 
Config::run(int argc, char* argv[]) 
{
    return _pImpl->run(argc, argv);
}
// END: class Config

