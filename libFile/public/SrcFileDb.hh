
#pragma once

#include <memory>   // for std::unique_ptr
#include <string> 
#include <vector> 

class SrcLine {
public:
    enum class LineInfo {
        EMPTY           =   1   <<  0,      
        POND_INCLUDE    =   1   <<  1,      // #incldue <...>
        POND_OTHERS     =   1   <<  2,      // #if/#define ...: No rule will be applied
        COMMENT_SINGLE  =   1   <<  3,      // single line comment  '//'
        COMMENT_START   =   1   <<  4,      // contains start of comment '/*'
        COMMENT_END     =   1   <<  5,      // contains end of comment '*/'
        NAMESPACE       =   1   <<  6,      // 
        TEMPLATE        =   1   <<  7,      // 
        CLASS           =   1   <<  8,
        SCOPE_START     =   1   <<  9,      // Begin a new scope: indent would be same as previous line
        SCOPE_END       =   1   <<  10,     // End a scope
        SCOPE_MIDDLE    =   1   <<  11      // In Middle of some scope. indent = parent
    };
public:
    size_t      _leadingTab;
    size_t      _leadingSpace;
    LineInfo    _info;
    SrcLine*    _parent;
};

class SrcFile {
public:
    SrcFile() {}
    SrcFile(const std::string& fileName)    :   _fileName(fileName) {}
private:
    std::vector<SrcLine> _srcLines;
    std::string          _fileName;
};

SrcLine::LineInfo operator|   (SrcLine::LineInfo lhs, SrcLine::LineInfo rhs);

