#include <string>
#include "format.h"

using std::string;

// Format an integer as an (at least) two digit string
string Format2d(int i) {
    return (i< 10 ? "0" : "") + std::to_string(i);
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    string hh = Format2d(   seconds / 3600 );
    string mm = Format2d( ( seconds % 3600 ) / 60 );
    string ss = Format2d( ( seconds % 3600 ) % 60 );
    return hh + ":" + mm + ":" + ss;
}