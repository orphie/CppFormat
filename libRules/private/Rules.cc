#include <iostream>
#include "libRules/public/Rules.hh"
#include "libConfig/public/Config.hh"

using namespace std;

// class RulesImpl
class Rules::RulesImpl {
public:
    RulesImpl()    = default;
    ~RulesImpl()   = default;
public:
    int run(const Config&);
};

int 
Rules::RulesImpl::run(const Config& c) {
    return 0;
}
// END: class RulesImpl


// class Rules
Rules::Rules() :  _pImpl{std::make_unique<RulesImpl>()}
{}

Rules::~Rules() = default;

int 
Rules::run(const Config& c)
{
    return _pImpl->run(c);
}
// END: class Rules

