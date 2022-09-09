#include "libUtils/public/Timer.hh"

Time::Time()
: _wallTime(0)
, _cpuTime()
{
    _wallTime = times(&(_cpuTime));
}

