#if defined(linux) || defined(__linux) || defined(__linux__)
    #define JOC_LINUX_BUILD
#elif defined(_WIN32) || defined(WIN32)
    #define JOC_WINDOWS_BUILD
#else
    #error Platform not supported. Please notify evanallan1026@gmail.com
#endif
