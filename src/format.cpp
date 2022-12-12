#include "format.h"

#include <string>
#include <sstream>

using std::string; 

string Format::ElapsedTime(long seconds) { 
    
    std::ostringstream ss;
    int h{0}, m{0}, s{0};
    h = (seconds/3600);
    s = (seconds%3600);
    m = (s/60);
    s = (s % 60);
    
    ss.width(2);
    ss.fill('0');
    ss<<h<<":";
    ss.width(2);
    ss.fill('0');
    ss<<m<<":";
    ss.width(2);
    ss.fill('0');
    ss<<s;
    return ss.str();
}