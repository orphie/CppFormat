#pragma once

#include <memory>                       // for std::unique_ptr
#include <boost/program_options.hpp>    // for variables_map
class ConfigDb;
class ConfigAnalyzer {
public:
    ConfigAnalyzer(ConfigDb&);
    ~ConfigAnalyzer();
    void run(int argc, char* argv[]);
private:
    class ConfigAnalyzerImpl;
    std::unique_ptr<ConfigAnalyzerImpl> _pImpl;
};

