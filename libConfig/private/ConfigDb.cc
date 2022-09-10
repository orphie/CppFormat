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
    Options& getOptions()   { return _options; }
private:
    Options _options;
};

std::string
ConfigDb::ConfigDbImpl::getDiffFileName() const {
    return _options["diff"].as<std::string>();
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

std::string 
ConfigDb::getDiffFileName()   const
{
    return _pImpl->getDiffFileName();
}

ConfigDb::Options&
ConfigDb::getOptions()
{
    return _pImpl->getOptions();
}
// END: class ConfigDb

