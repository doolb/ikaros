#ifndef LOG2_H
#define LOG2_H
#include "config.h"

/* Integer base 2 logarithm calculation
 *
 * Copyright (C) 2006 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

 // Determine whether some value is a power of two, where zero is *not* considered a power of two.
static inline int is_pow_2 (ulong n) {
	return (n != 0) && ((n & (n - 1)) == 0);
}

#define pow2(n) (1 << n)

static int ilog2 (ulong n) {
	if (n < 1) return 0;
	for (int i = 4 * sizeof (n) - 1; i >= 0; i--) {
		if (n & 1ULL << i) return i;
	}
	return 0;
}

#define pow2_ff(n) ((1ULL << n) - 1)

#endif
