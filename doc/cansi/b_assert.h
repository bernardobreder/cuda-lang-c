#ifndef BREDER_ASSERT_H
#define BREDER_ASSERT_H

#include <assert.h>

/**
 * Assert the true value. If the value is not true, the process will finish.
 * This implementation will be ignore by pre-processor if the environment is Release (not Debug).
 */
#define b_assert(TRUE) assert(TRUE)

#endif
