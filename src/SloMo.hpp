#ifndef SLOMO_SLOMO_HPP
#define SLOMO_SLOMO_HPP

#include <string>

using std::string;

namespace slomo
{
    class SloMo
    {

    public:
        void slowdown(const string &inFilename, const string outFilename);
    };
}

#endif