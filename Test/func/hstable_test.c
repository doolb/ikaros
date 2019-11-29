#include "test.h"

#define No_Debug
//#define TRACE_HASH_CONFLICATED
#include "hashtable.h"
#include <time.h>
#include "System.h"
#include "System/Collections.h"

typedef struct _mystruct {
	int data;
	char* sdata;
	Def_HashList;
}mystruct, *pmystruct;

#define hash_bit 16
test_start (hstable) {
	mem_init();
	HashTable a = null;

// 	printf ("hash_min: %d, %lu\n", 3, hash_min (3, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", -1, hash_min (-1, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 323423, hash_min (323423, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 3242353, hash_min (3242353, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 34534345, hash_min (34534345, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 1, hash_min (1, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 2, hash_min (2, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 3, hash_min (3, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 4, hash_min (4, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 5, hash_min (5, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 6, hash_min (6, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 7, hash_min (7, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 8, hash_min (8, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 9, hash_min (9, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 0, hash_min (0, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 10, hash_min (10, Hash_Bits (a)));
// 	printf ("hash_min: %d, %lu\n", 20, hash_min (20, Hash_Bits (a)));

#define HASH(str) printf ("hash_min_string: %s, %llx\n", str, hash_str (str, sizeof(str)))
	HASH("123");
	HASH("sd232hfsk");
	HASH("==sdh26sfsk");
	HASH("sdhfsdfsk");
	HASH("nb");

	int pr = GetPrime(0);
	do {
		printf("%d\n", pr);
		pr = ExpandPrime(pr);
	} while (pr < Prime_Max && pr > 0);
	printf("%d\n", pr);

	mystruct first = {
		.data = 10,
		.sdata = "==sdh26sfsk",
		Def_HashList_Init
	};

	mystruct second = null;
	second.data = 20;
	second.sdata = "1a23";
	hlistInit (&second);

	_(HashTable).Add(&a, "==sdh26sfsk", sizeof("==sdh26sfsk"), &first._list_);
	_(HashTable).Add(&a, "1a23", sizeof("1a23"), &second._list_);

 	int bkt;
 	mystruct *cur;
 	printf("itera manual.\n");
 	for (bkt = 0; bkt < a.max; bkt++) {
		printf("%d: ", bkt);
#ifdef TRACE_HASH_CONFLICATED
		printf(" -- %d : ", a.item_count[bkt]);
#endif
		if (a.items[bkt] == NULL) {
			printf("\n");
			continue;
		}
		for (cur = containof(a.items[bkt], mystruct, _list_);
			;
			cur = containof(cur->_list_.next, mystruct, _list_)) {
			printf("data: %d @ %d %s", cur->data, bkt, cur->sdata);
			if ((cur->_list_.next) == NULL)
				break;
		}
		printf("\n");
	}
 	printf("itera with Hash_for_each_entry.\n");
	Hash_for_each(&a, bkt, mystruct, cur, {
		printf("data: %d @ %d %s\n", cur->data, bkt, cur->sdata);
		});
 		

	int num = 100000000;
	size_t size = num * sizeof(mystruct);
	size_t k = size / 1024;
	size_t m = k / 1024;
	size_t g = m / 1024;
	printf("count:%d %d %dG %dM %dK\n", num, sizeof(mystruct), g, m, k);
	RealElapsedTime();
	mystruct** p = mmalloc(num * sizeof(mystruct*));
	if (p == NULL)
		exit(1);
	for (int i = 0; i < num; i++) {
		p[i] = mmalloc(sizeof(mystruct));
		if(p[i] == NULL)
			exit(1);
	}
	printf("alloc cpu time: %lf ms \n", RealElapsedTime());

	RealElapsedTime();
	long l = 0;
	for (int i = 0; i < num; i++) {
		l++;
	}
	printf("array cpu time: %lf ms \n", RealElapsedTime());

	RealElapsedTime();
	for (int i = 0; i < num; i++) {
		hlistInit(p[i]);
		p[i]->data = i;
		p[i]->sdata = 0;
		_(HashTable).Add(&a, i, 0, &(p[i]->_list_));
	}
	printf("insert cpu time: %lf ms \n", RealElapsedTime());
#ifdef TRACE_HASH_CONFLICATED
	printf("max conflicated : %d \n", a.max_conflicated);
#endif
	RealElapsedTime();
	Hash_for_each(&a, bkt, mystruct, cur, {
			;// printf("data: %d @ %d %s\n", cur->data, bkt, cur->sdata);
		});
	printf("itera cpu time: %lf ms \n", RealElapsedTime());

	RealElapsedTime();
	LARGE_INTEGER  large_interger;
	double dff;
	__int64  c1, c2;
	QueryPerformanceFrequency(&large_interger);
	dff = large_interger.QuadPart;
	QueryPerformanceCounter(&large_interger);
	c1 = large_interger.QuadPart;

	for (int i = 0; i < num; i++) {
		//void* ptr = _(HashTable).Get(&a, i, 0);
		void* ptr = hashGet(&a, (void*)i, 0);
		if (ptr != NULL) {
			;// cur = containof(ptr, mystruct, _list_);
			;// printf("data: %d @ %d %s\n", cur->data, bkt, cur->sdata);
		}
	}

	QueryPerformanceCounter(&large_interger);
	c2 = large_interger.QuadPart;
	printf("本机高精度计时器频率%lf\n", dff);
	printf("第一次计时器值%I64d 第二次计时器值%I64d 计时器差%I64d\n", c1, c2, c2 - c1);
	printf("计时%lf毫秒\n", (c2 - c1) * 1000 / dff);

	printf("access cpu time: %lf ms \n", RealElapsedTime());

   	mem_clear();
}
test_end ()