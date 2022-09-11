#include <iostream>
#include <fstream>
#include "libConfig/public/ConfigDb.hh"
#include "libConfig/public/ConfigAnalyzer.hh"
#include "libUtils/public/Timer.hh"

using namespace std;
using namespace boost::program_options;


// class ConfigDbImpl
class ConfigDb::ConfigDbImpl {
public:
    using Base  = ConfigDb;  
    //using Options = Base::Options;
    using Options = boost::program_options::variables_map;
    ConfigDbImpl()    = default;
    ~ConfigDbImpl()   = default;
public:
    // accessories
    bool hasDiffFile()              const   { return _options.count("diff"); }
    std::string getDiffFileName()       const;

    bool hasInputFile()             const   { return _options.count("file"); }
    std::vector<std::string> getInputFileNames()   const;
    Options& getOptions()   { return _options; }
private:
    Options _options;
};

std::string
ConfigDb::ConfigDbImpl::getDiffFileName() const {
    return _options["diff"].as<std::string>();
}

std::vector<std::string> 
ConfigDb::ConfigDbImpl::getInputFileNames()   const
{
    return _options["file"].as< std::vector<std::string> >();
}



// END: class ConfigDbImpl


// class ConfigDb
ConfigDb::ConfigDb() :  _pImpl{std::make_unique<ConfigDbImpl>()}
{}

ConfigDb::~ConfigDb() = default;

bool 
ConfigDb::hasDiffFile()   const
{
    return _pImpl->hasDiffFile();
}

bool 
ConfigDb::hasInputFile()   const
{
    return _pImpl->hasInputFile();
}

std::string 
ConfigDb::getDiffFileName()   const
{
    return _pImpl->getDiffFileName();
}

std::vector<std::string> 
ConfigDb::getInputFileNames()   const
{
    return _pImpl->getInputFileNames();
}

ConfigDb::Options&
ConfigDb::getOptions()
{
    return _pImpl->getOptions();
}
// END: class ConfigDb

