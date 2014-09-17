#ifndef PLATFORM_H_
#define PLATFORM_H_

#if __cplusplus >= 201103L
#define CPP11
#else
#define CPPTR1
#endif

#if __STDC_VERSION__ >= 201112L
#define C11
#endif

#if defined(C11)
#define WithStdInt
#endif

#if defined(CPP11)
#define WithMemory
#define WithArray
#endif // CPP11

#if defined(CPPTR1)
#define WithStdInt

#if defined(WIN32)
#define WithMemory
#define WithArray

#if defined(WIN32)
#define snprintf _snprintf
#endif

#else
#define WithTr1Memory
#define WithTr1Array
#endif // defined(WIN32)

#endif // CPPTR1

#if !defined(WithMemory) && !defined(WithTr1Memory)
#define WithBoostMemory
#endif

#if !defined(WithArray) && !defined(WithTr1Array)
#define WithBoostArray
#endif

// Implementations below here
#if defined(WithMemory)
#include <memory>
#endif

#if defined(WithArray)
#include <array>
#endif

#if defined(WithStdInt)
#include <cstdint>
#endif

#if defined(WithTr1Array)
#include <tr1/memory>
#endif

#if defined(WithTr1Array)
#include <tr1/array>
namespace std {
    using std::tr1::array;
}
#endif

#if defined(WithBoostArray)
#include <boost/array.hpp>
namespace std {
    using boost::array;
}
#endif

#if defined(WithBoostMemory)
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
namespace std {
    using boost::shared_ptr;
    using boost::weak_ptr;
}
#endif

#if !defined(WithStdInt)
typedef unsigned long long int uint64_t;
typedef unsigned int      uint32_t;
typedef unsigned short int     uint16_t;
typedef unsigned char      uint8_t;

typedef signed long long int int64_t;
typedef signed int     int32_t;
typedef signed short int    int16_t;
typedef signed char      int8_t;
#endif

#endif /* PLATFORM_H_ */
