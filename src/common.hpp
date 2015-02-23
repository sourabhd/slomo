#ifndef SLOMO_COMMON_HPP
#define SLOMO_COMMON_HPP

#define CERR std::cerr << __FILE__ << ":" << __LINE__ << ":" << __func__ << ":: "

#define TIME_START(x) \
    clock_t clock##x##_start = clock(); \
    time_t time##x##_start = time(NULL);

#define TIME_END(x, msg) \
    clock_t clock##x##_end = clock(); \
    time_t time##x##_end = time(NULL); \
    std::cerr << __FILE__ << ":" << __LINE__ << ":" << __func__ << ":: " \
    << "Time taken for :" << msg << " in seconds : " \
    << double(clock##x##_end-clock##x##_start) / CLOCKS_PER_SEC << " (processor) " \
    << (double(time##x##_end)-double(time##x##_start)) << " (walltime)" \
    << endl << flush;

#endif