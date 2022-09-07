#pragma once

#include <memory>                       // for std::unique_ptr
#include <boost/program_options.hpp>    // for variables_map
class OptionParser {
public:
    using Options = boost::program_options::variables_map;
    OptionParser(Options&);
    ~OptionParser();
    void run(int argc, char* argv[]);
private:
    class OptionParserImpl;
    std::unique_ptr<OptionParserImpl> _pImpl;
};

