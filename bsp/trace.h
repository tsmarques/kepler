#ifndef KEPLER_TRACE_H
#define KEPLER_TRACE_H

#if defined(ARM_SEMIHOST)
#  include <cstdio>
#  define trace(...) printf(__VA_ARGS__)
#else
#  define trace(...)
#endif

#endif
