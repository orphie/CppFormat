#include <vector>
#include <cassert>
#include "libFile/public/SrcFileDb.hh"

SrcLine::LineInfo
operator|   (SrcLine::LineInfo lhs, SrcLine::LineInfo rhs)
{
    size_t ret = (size_t)lhs | (size_t)rhs;
    return SrcLine::LineInfo(ret);
}

