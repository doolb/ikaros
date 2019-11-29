#ifndef HASH_HELPERS_H
#define HASH_HELPERS_H
int IsPrime(int candidate);

#if! HASH_HELPERS
#include <math.h>
int IsPrime(int candidate) {
	// can div by 2
	if ((candidate & 1) != 0) {
		// test 3 to sqrt(N)
		int num = (int)sqrt(candidate);
		for (int i = 3; i <= num; i += 2)
			if (candidate % i == 0)
				return false;
		return true;
	}
	// only 2 is prime
	return candidate == 2;
}

static int primes[] = {
		3,
		7,
		11,
		17,
		23,
		29,
		37,
		47,
		59,
		71,
		89,
		107,
		131,
		163,
		197,
		239,
		293,
		353,
		431,
		521,
		631,
		761,
		919,
		1103,
		1327,
		1597,
		1931,
		2333,
		2801,
		3371,
		4049,
		4861,
		5839,
		7013,
		8419,
		10103,
		12143,
		14591,
		17519,
		21023,
		25229,
		30293,
		36353,
		43627,
		52361,
		62851,
		75431,
		90523,
		108631,
		130363,
		156437,
		187751,
		225307,
		270371,
		324449,
		389357,
		467237,
		560689,
		672827,
		807403,
		968897,
		1162687,
		1395263,
		1674319,
		2009191,
		2411033,
		2893249,
		3471899,
		4166287,
		4999559,
		5999471,
		7199369
};
#define primes_length (sizeof(primes) / sizeof(primes[0]))
#define Prime_Max 2147483647 
int GetPrime(int min) {
	if (min < 0)
		return -1;
	for (int i = 0; i < primes_length; i++) {
		if (primes[i] > min)
			return primes[i];
	}
	for (int i = min | 1; i < Prime_Max; i += 2) {
		if (IsPrime(i) && (i - 1) % 101 != 0)
			return i;
	}
	return min;
}

int ExpandPrime(int oldSize) {
	uint num = 2 * oldSize;
	if (num >= Prime_Max && Prime_Max >= oldSize)
		return Prime_Max;
	return GetPrime(num);
}
#undef primes_length
#endif


#endif
