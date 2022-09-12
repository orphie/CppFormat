#include "libConfig/public/ConfigDb.hh"
#include "libConfig/public/ConfigAnalyzer.hh"

#include "libFile/public/FileDb.hh"
#include "libFile/public/FileAnalyzer.hh"

#include "libRule/public/RuleDb.hh"
#include "libRule/public/RuleAnalyzer.hh"

#include "libFormat/public/Format.hh"
#include "libUtils/public/Timer.hh"

int main(int argc, char* argv[])
{
    ScopeTimer timer("Main");

    ConfigDb config;
    {
        ScopeTimer timer("Analyze Config");
        ConfigAnalyzer(config).run(argc, argv);
    }

    FileDb file;
    {
        ScopeTimer timer("Analyze Input Files");
        FileAnalyzer(file).run(config);
    }

    RuleDb rule;
    {
        ScopeTimer timer("Extract Formatting Rules");
        RuleAnalyzer(rule).run(config, file);
    }

    {
        ScopeTimer timer("Apply Rules On Input Files");
        Format(config, rule, file).run();
    }

    return 0;
}

