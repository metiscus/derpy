#ifndef PLATFORM_H_
#define PLATFORM_H_

#if __cplusplus >= 201103L
#define CPP11
#else
#define CPPTR1 1
#endif

#if defined(CPP11)
#include <memory>
#elif defined(CPPTR1)

#if defined(WIN32)
#include <memory>
#else
#include <tr1/memory>
#endif

namespace std {
    using std::tr1::shared_ptr;
    using std::tr1::weak_ptr;
}

#else
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace std {
    using boost::shared_ptr;
    using boost::weak_ptr;
}

#endif

#endif /* PLATFORM_H_ */
