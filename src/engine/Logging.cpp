#include "Logging.h"
#include <cassert>

//! LogImpl will write the arguments to stderr
void LogImpl(int level, const char *function, const char *format, ...)
{
    va_list args;
    va_start(args, format);

    const char *levelTag[] = { "", "(Fatal)", "(Error)", "(Warning)",
                               "(Debug)", "(Info)", "(Trace)" };

    if (level <= LOG_LEVEL)
    {
#if defined(__GNUC__)
        char *saveptr;
        char *tmpStr = strdup(function);
        char *className = strtok_r(tmpStr, "(", &saveptr);
#elif defined(WIN32)
        char *tmpStr = strdup(function);
        char *className = strtok(tmpStr, "(");
#endif
        fprintf(stderr, "%s [%s] ", levelTag[level], className);
        vfprintf(stderr, format, args);
        fprintf(stderr, "\n");

        free(tmpStr);
    }

    va_end(args);
}
