#include <iostream>
#include "libDatabase/public/FormatDb.hh"
#include "libConfig/public/Config.hh"
#include "libRules/public/Rules.hh"
#include "libFormat/public/Format.hh"

using namespace std;
// class FormatDbImpl
class FormatDb::FormatDbImpl {
public:
    FormatDbImpl()    = default;
    ~FormatDbImpl()   = default;
public:
    int run(int argc, char* argv[]);
};

int 
FormatDb::FormatDbImpl::run(int argc, char* argv[]) {
    // analyze configs
    Config config;
    config.run(argc, argv);

    // extract rules from config
    Rules rules;
    rules.run(config);

    // Apply Rules on Files/Functions/Lines
    Format().run(config, rules);

    // analyze input files
    return 0;
}
// END: class FormatDbImpl


// class FormatDb
FormatDb::FormatDb() :  _pImpl{std::make_unique<FormatDbImpl>()}
{}

FormatDb::~FormatDb() = default;

int 
FormatDb::run(int argc, char* argv[]) 
{
    return _pImpl->run(argc, argv);
}
// END: class FormatDb

