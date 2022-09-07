#pragma once

#include <memory>   // for std::unique_ptr

class FormatDb {
public:
    FormatDb();
    ~FormatDb();
    int run(int argc, char* argv[]);
private:
    class FormatDbImpl;
    std::unique_ptr<FormatDbImpl> _pImpl;
};
