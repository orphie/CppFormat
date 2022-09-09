#include <iostream>
#include <sys/times.h>

class Time {
    using SELF = Time;
public:
    Time();
public:
    double getWallTime() const { return static_cast<double>(_wallTime); }
    double getUserTime() const { return static_cast<double>(_cpuTime.tms_utime); }
    double getSysTime() const { return static_cast<double>(_cpuTime.tms_stime); }
    double getWallTimeInSecond() const { return getWallTime() / 100.0; }
    double getUserTimeInSecond() const { return getUserTime() / 100.0; }
    double getSysTimeInSecond() const { return getSysTime() / 100.0; }
private:
    clock_t    _wallTime;
    struct tms _cpuTime;
};


class ScopeTimer
{
public:
    using SELF = ScopeTimer;
    ScopeTimer(const std::string& s) :
            _step(s),
            _startTime()
    {
        std::cout << "#Step " << step() << " : Starting..." << std::endl;
    }

    ~ScopeTimer() {
        Time currTime;
        std::cout << "#Step "   << step() << " : Done in\t" 
                  << "wall:"    << getWallTime(currTime) << "s,  " 
                  << "sys:"     << getSysTime(currTime)  << "s,  "
                  << "user:"    << getUserTime(currTime) << "s" << std::endl;
    }

public:
    const std::string& step() const {
        return _step;
    }
    double getWallTime(const Time& currTime) const {
        return (currTime.getWallTimeInSecond() - _startTime.getWallTimeInSecond());
    }
    double getSysTime(const Time& currTime) const {
        return (currTime.getSysTimeInSecond() - _startTime.getSysTimeInSecond());
    }
    double getUserTime(const Time& currTime) const {
        return (currTime.getUserTimeInSecond() - _startTime.getUserTimeInSecond());
    }
private:
    std::string    _step;
    Time           _startTime;

private:
    SELF& operator=(const SELF& o)  = delete;
    ScopeTimer(const SELF& o)       = delete;
};

