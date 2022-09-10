#include <iostream>
#include "libRule/public/RuleAnalyzer.hh"
#include "libUtils/public/Timer.hh"
using namespace std;

class RuleAnalyzer::RuleAnalyzerImpl {
public:
    using Base      =   RuleAnalyzer;
    RuleAnalyzerImpl(RuleDb& rule)    :   _rule(rule) {}
public:
    void run(const ConfigDb&, const FileDb&) {}
private:
    RuleDb&     _rule;
};

// class RuleAnalyzer
RuleAnalyzer::RuleAnalyzer(RuleDb& rule) :  _pImpl{std::make_unique<RuleAnalyzerImpl>(rule)}
{}

RuleAnalyzer::~RuleAnalyzer() = default;

void
RuleAnalyzer::run(const ConfigDb& config, const FileDb& file)
{
    return _pImpl->run(config, file);
}
// END: class RuleAnalyzer


