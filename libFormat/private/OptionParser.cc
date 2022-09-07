#include <iostream>
#include <boost/program_options.hpp>
#include "libConfig/private/OptionParser.hh"
using namespace boost::program_options;
using namespace std;

class OptionParser::OptionParserImpl {
public:
    using Base      =   OptionParser;
    using Options   =   Base::Options;
    OptionParserImpl(Options& options)    :   _options(options) {}
public:
    void run(int argc, char* argv[]) {
        options_description desc{"Options"};
        using StrVec = std::vector<std::string>;
        desc.add_options()
            ("help,h", "Print this help")
            ("thread,t",  value<int>()->default_value(INT_MAX),   "Number of threads")
            ("file,f",    value<StrVec>(),                        "Input files")
            ("config,c",  value<StrVec>(),                        "Configs")
            ("diff,d",    value<std::string>(),                   "Diff file");

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
private:
    Options& _options;
};

// class OptionParser
OptionParser::OptionParser(OptionParser::Options& options) :  _pImpl{std::make_unique<OptionParserImpl>(options)}
{}

OptionParser::~OptionParser() = default;

void 
OptionParser::run(int argc, char* argv[]) 
{
    return _pImpl->run(argc, argv);
}
// END: class OptionParser

