#include "../TimeSpan.h"

using namespace GNAFramework;

TimeSpan::TimeSpan() {
    this->total_clocks = 0;
}

TimeSpan::TimeSpan(clock_t clocks) {
    setTo(clocks);
}

TimeSpan::TimeSpan(int seconds, int milliseconds){
    setTo(seconds, milliseconds);
}



void TimeSpan::setTo(clock_t clocks) {
    total_clocks = clocks;
}

void TimeSpan::setTo(int seconds, int milliseconds) {
    total_clocks = (long long int) ((seconds + milliseconds / (double)1000) * CLOCKS_PER_SEC);
}



TimeSpan TimeSpan::operator+(const TimeSpan &other) const {
    TimeSpan result = *this; // Make a copy of myself.;
    result.total_clocks += other.total_clocks;
    return result;
}

TimeSpan TimeSpan::operator-(const TimeSpan &other) const {
    TimeSpan result = *this; // Make a copy of myself.;
    result.total_clocks -= other.total_clocks;
    return result;
}



bool TimeSpan::operator==(const TimeSpan &other) const{
    return total_clocks == other.total_clocks;
}

bool TimeSpan::operator>(const TimeSpan &other) const{
    return total_clocks > other.total_clocks;
}

bool TimeSpan::operator>=(const TimeSpan &other) const{
    return total_clocks >= other.total_clocks;
}

bool TimeSpan::operator<(const TimeSpan &other) const{
    return total_clocks < other.total_clocks;
}

bool TimeSpan::operator<=(const TimeSpan &other) const{
    return total_clocks <= other.total_clocks;
}



long long int TimeSpan::toTicks() const{
    return this->total_clocks;
}

double TimeSpan::toTicksD() const{
    return (double) this->total_clocks;
}

float TimeSpan::toSecondsF()  const{
    return float(((double) this->total_clocks) / ((double) CLOCKS_PER_SEC));
}

float TimeSpan::toMillisecondsF() const {
    return (float) this->total_clocks * (1000 / (float) CLOCKS_PER_SEC);
}

double TimeSpan::toSecondsD()  const{
    return (double) this->total_clocks / (double) CLOCKS_PER_SEC;
}

double TimeSpan::toMillisecondsD()  const{
    return (double) this->total_clocks * (1000 / (double) CLOCKS_PER_SEC);
}

//TimeSpan::TimeSpan(const TimeSpan& orig) {}

TimeSpan::~TimeSpan() {
}

