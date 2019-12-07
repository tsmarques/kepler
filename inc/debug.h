#ifndef KEPLER_DEBUG_H
#define KEPLER_DEBUG_H

#if defined(ARM_SEMIHOST)
#  warning "Compiling with ARM_SEMIHOST"
#  include <stdio.h>
#  define debug_init() initialise_monitor_handles()
#  define trace(...) printf(__VA_ARGS__)
#else
#  define debug_init()
#  define trace(...);
#endif

#endif
