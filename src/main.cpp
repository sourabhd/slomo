#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>
#include "common.hpp"
#include "SloMo.hpp"

using namespace std;
using namespace slomo;

static void usageMsg(void)
{
    cerr << "Usage: slomo <videofile> <outfile>" << endl;
}

int main(int argc, char *argv[])
{
    try {

        if (argc != 3) {
            usageMsg();
            return 1;
        }

        string inVideoFilename(argv[1]);
        string outVideoFilename(argv[2]);

        SloMo sm;
        sm.slowdown(inVideoFilename, outVideoFilename);

    } catch (exception &e) {
        cerr << e.what() << endl;
        CERR << "exception in main" << endl;
    }

    return 0;
}