#pragma once
#include <vector>
#include <boost/program_options.hpp>    // for variables_map

//namespace boost::program_options {
//class variables_map;
//}

#include <memory>   // for std::unique_ptr
class ConfigDb {
public:
    using Options = boost::program_options::variables_map;
    ConfigDb();
    ~ConfigDb();
public:
    // accessories
    bool hasDiffFile()              const;
    std::string getDiffFileName()   const;

    bool hasInputFile()             const;
    std::vector<std::string> getInputFileNames()   const;
private:
    Options& getOptions();
private:
    class ConfigDbImpl;
    std::unique_ptr<ConfigDbImpl> _pImpl;
    friend class ConfigAnalyzer;
};

