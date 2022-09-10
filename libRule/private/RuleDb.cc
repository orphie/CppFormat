#include <iostream>
#include "libRule/public/RuleDb.hh"
#include "libConfig/public/ConfigDb.hh"

using namespace std;

// class RuleDbImpl
class RuleDb::RuleDbImpl {
public:
    RuleDbImpl()    = default;
    ~RuleDbImpl()   = default;
public:
    int run(const Config&);
};

int 
RuleDb::RuleDbImpl::run(const Config& c) {
    //IndentRule ir;
    return 0;
}
// END: class RuleDbImpl


// class RuleDb
RuleDb::RuleDb() :  _pImpl{std::make_unique<RuleDbImpl>()}
{}

RuleDb::~RuleDb() = default;

int 
RuleDb::run(const Config& c)
{
    return _pImpl->run(c);
}
// END: class RuleDb

