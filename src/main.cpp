#include <iostream>
#include <cstdlib>
#include <string>
#include <exception>
#include <ctime>
#include <cstring>
#include "common.hpp"
#include "SloMo.hpp"

using namespace std;
using namespace slomo;

static void usageMsg(void)
{
    cerr << "Usage: slomo <videofile> <outfile> <slowdownfactor>" << endl;
}

int main(int argc, char *argv[])
{
    try {

        TIME_START(1)
        //clock_t clock1_start = clock();
        //time_t time1_start = time(NULL);

        if (argc != 4) {
            usageMsg();
            return 1;
        }

        string inVideoFilename(argv[1]);
        string outVideoFilename(argv[2]);
        const int factor = atoi(argv[3]);

        if (outVideoFilename.find(".avi") != outVideoFilename.length()-4) {
            cerr << "This program can generate only AVI output files" << endl;
            cerr << "Use a program like ffmpeg to convert from AVI to required format" << endl;
            return 1;
        }

        SloMo sm;
        sm.slowdown(inVideoFilename, outVideoFilename, factor);

        TIME_END(1, "SloMo")

    } catch (exception &e) {
        cerr << e.what() << endl;
        CERR << "exception in main" << endl;
    }

    return 0;
}