#ifndef PLATFORM_H_
#define PLATFORM_H_

#if __cplusplus >= 201103L
#define CPP11
#else
#define CPPTR1 1
#endif

#if defined(CPP11)
#include <memory>
#include <array>
#elif defined(CPPTR1)

#if defined(WIN32)
#include <memory>
#include <array>
#else
#include <tr1/memory>
#include <tr1/array>
#endif

namespace std {
    //using std::tr1::shared_ptr;
    //using std::tr1::weak_ptr;
    using std::tr1::array;
}

#else
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/array.hpp>

namespace std {
    using boost::shared_ptr;
    using boost::weak_ptr;
    using std::tr1::array;
}

#endif

#endif /* PLATFORM_H_ */
