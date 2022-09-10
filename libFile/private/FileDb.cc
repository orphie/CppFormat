#include <iostream>
#include "libFile/public/FileDb.hh"
#include "libConfig/public/ConfigDb.hh"

using namespace std;

// class FileDbImpl
class FileDb::FileDbImpl {
public:
    FileDbImpl()    = default;
    ~FileDbImpl()   = default;
private:
};

//int 
//FileDb::FileDbImpl::run(const Config& c) {
//    return 0;
//}
// END: class FileDbImpl


// class FileDb
FileDb::FileDb() :  _pImpl{std::make_unique<FileDbImpl>()}
{}

FileDb::~FileDb() = default;

//int 
//FileDb::run(const Config& c)
//{
//    return _pImpl->run(c);
//}
// END: class FileDb

