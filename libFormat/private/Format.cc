#include <iostream>
#include "libFormat/public/Format.hh"
#include "libConfig/public/ConfigDb.hh"
#include "libRule/public/RuleDb.hh"
#include "libFile/public/FileDb.hh"

using namespace std;
// class FormatImpl
class Format::FormatImpl {
public:
    FormatImpl(const ConfigDb&, const RuleDb&, const FileDb&) {}
    ~FormatImpl()   = default;
public:
    int run();
};

int 
Format::FormatImpl::run() {
    return 0;
}
// END: class FormatImpl


// class Format
Format::Format(const ConfigDb& c, const RuleDb& r, const FileDb& f) : _pImpl{std::make_unique<FormatImpl>(c,r,f)}
{}

Format::~Format() = default;

int Format::run()
{
    return _pImpl->run();
}
// END: class Format

