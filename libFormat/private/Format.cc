#include <iostream>
#include "libFormat/public/Format.hh"
#include "libConfig/public/Config.hh"
#include "libRules/public/Rules.hh"

using namespace std;
// class FormatImpl
class Format::FormatImpl {
public:
    FormatImpl()    = default;
    ~FormatImpl()   = default;
public:
    int run(const Config&, const Rules&);
};

int 
Format::FormatImpl::run(const Config& c, const Rules& r) {
    return 0;
}
// END: class FormatImpl


// class Format
Format::Format() :  _pImpl{std::make_unique<FormatImpl>()}
{}

Format::~Format() = default;

int 
Format::run(const Config& c, const Rules& r)
{
    return _pImpl->run(c, r);
}
// END: class Format

