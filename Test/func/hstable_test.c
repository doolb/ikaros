#include "test.h"
#include "hashtable.h"


typedef struct _mystruct {
	int data;
	char* sdata;
	Def_HashList
}mystruct;

test_start (hstable) {

	Def_HashTable (a, 3);

	printf ("hash_min: %d, %lu\n", 3, hash_min (3, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", -1, hash_min (-1, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 323423, hash_min (323423, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 3242353, hash_min (3242353, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 34534345, hash_min (34534345, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 1, hash_min (1, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 2, hash_min (2, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 3, hash_min (3, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 4, hash_min (4, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 5, hash_min (5, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 6, hash_min (6, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 7, hash_min (7, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 8, hash_min (8, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 9, hash_min (9, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 0, hash_min (0, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 10, hash_min (10, Hash_Bits (a)));
	printf ("hash_min: %d, %lu\n", 20, hash_min (20, Hash_Bits (a)));

	printf ("hash_min_string: %s, %llx\n", "1a23", hash_str_bit ("1a23", HS_LEN, 3));
	printf ("hash_min_string: %s, %llx\n", "sd232hfsk", hash_str_bit ("sd232hfsk", HS_LEN, 3));
	printf ("hash_min_string: %s, %llx\n", "==sdh26sfsk", hash_str_bit ("==sdh26sfsk", HS_LEN, 3));
	printf ("hash_min_string: %s, %llx\n", "sdhfsdfsk", hash_str_bit ("sdhfsdfsk", HS_LEN, 3));
	printf ("hash_min_string: %s, %llx\n", "nb", hash_str_bit ("nb", HS_LEN, 3));



	mystruct first = {
		.data = 10,
		.sdata = "==sdh26sfsk",
		Def_HashList_Init
	};

	mystruct second = null;
	second.data = 20;
	second.sdata = "1a23";
	hlistInit (&second);

	hashAdds (a, &first, first.sdata, HS_LEN);
	hashAdds (a, &second, second.sdata, HS_LEN);

	int bkt;
	mystruct *cur;
	// 	for (int bkt = 0; bkt < Array_Size (a); bkt++) {
	// 		Hlist_for_each_entry (mystruct, cur, &a [bkt])
	// 			printf ("data: %d @ %d %s\n", cur->data, bkt, cur->sdata);
	// 	}
	Hash_for_each_entry (a, bkt, mystruct, cur)
		printf ("data: %d @ %d %s\n", cur->data, bkt, cur->sdata);
}
test_end ()