#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <assert.h>
#include "config.h"
#include "log2.h"

// hashtable implements
// refer:https://kernelnewbies.org/FAQ/Hashtables
// insert the struct in you own data-struct
typedef struct _HashList {
	struct _HashList *next; // point to next entry
}HashList, *pHashList;

// define a HashList in you own struct
#define Def_HashList    HashList _list_

// init the hash-list head when define
#define HList_Init			{0}
// init the hash-list head define with `Def_HashList`
#define Def_HashList_Init		._list_ = HList_Init

// init the hash-list head define with `Def_HashList` runtime
#define hlistInit(ptr)		_hlistInit(&((ptr)->_list_))
// init the list head runtime
static inline void _hlistInit (pHashList list) {
	list->next = NULL;
}
// define a hash table in struct
#define Def_HashTable(name, bits)						\
	HashList name[1 << (bits)]

// define a hash table
#define Def_HashTable_Init(name, bits)					\
	HashList name[1 << (bits)] = HList_Init

// get the hash-table bit-size
#define Hash_Bits(name)		ilog2(Array_Size(name))

// add a node define with `Def_HashList` to hash-table
#define hashAdd(hashtable, node, key)				\
	_hlistAdd (&((node)->_list_), &hashtable [hash_min(key,Hash_Bits(hashtable))])

#define HS_LEN 255
// add a node define with `Def_HashList` to hash-table, the key is string or any object
#define hashAdds(hashtable, node, key, max_len)		\
	_hlistAdd (&((node)->_list_), &hashtable [hash_str_bit (key, max_len, Hash_Bits (hashtable))])

// add node to hash-list head
static inline void _hlistAdd (pHashList node, pHashList head) {
	assert (node != head);
	node->next = head->next;
	head->next = node;
}

// iterate in a hash-list
#define Hlist_for_each_entry(Type,pos,head)							\
    for(pos = List_Entry((head)->next, Type, _list_);				\
        (&pos->_list_.next) != NULL;									\
        pos = List_Entry(pos->_list_.next, Type, _list_))

// iterate each entry in hash-table
#define Hash_for_each_entry(table, bkt, Type, obj)	\
	for((bkt)=0; (bkt) < Array_Size(table);	(bkt)++)\
		Hlist_for_each_entry(Type, obj, &table[bkt])


static inline void _hlistRemove (pHashList node, pHashList head) {
	assert (node != head);

}

#if 1 // hash function

/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define GOLDEN_RATIO_PRIME_32 0x9e370001UL
/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_PRIME_64 0x9e37fffffffc0001UL

#if defined (__i386__) || defined (WIN32)
#define GOLDEN_RATIO_PRIME GOLDEN_RATIO_PRIME_32
#define hash_long(val, bits) hash_32(val, bits)
#elif defined (__x86_64__) || defined (_WIN64)
#define hash_long(val, bits) hash_64(val, bits)
#define GOLDEN_RATIO_PRIME GOLDEN_RATIO_PRIME_64
#else
#error Wordsize not 32 or 64
#endif

static inline uint hash_32 (uint val, uint bits)
{
	/* On some cpus multiply is faster, on others gcc will do shifts */
	uint hash = val * GOLDEN_RATIO_PRIME_32;

	/* High bits are more random, so use them. */
	return hash >> (32 - bits);
}
static inline ulong hash_64 (ulong val, uint bits)
{
	ulong hash = val;

	/*  Sigh, gcc can't optimise this alone like it does for 32 bits. */
	ulong n = hash;
	n <<= 18;
	hash -= n;
	n <<= 33;
	hash -= n;
	n <<= 3;
	hash += n;
	n <<= 3;
	hash -= n;
	n <<= 4;
	hash += n;
	n <<= 2;
	hash += n;

	/* High bits are more random, so use them. */
	return hash >> (64 - bits);
}

#define hash_str	hash_djb2
#define hash_str_bit(str, max_len, bits) (hash_str(str, max_len) & pow2_ff(bits))

// hash for string, also for any object
// refer:http://www.cse.yorku.ca/~oz/hash.html
static inline unsigned long hash_djb2 (char *str, int max_len) {
	unsigned long hash = 5381;
	for (int i = 0; i < max_len && str [i]; i++)
		hash = ((hash << 5) + hash) + str [i];
	return hash;
}

/* Use hash_32 when possible to allow for fast 32bit hashing in 64bit kernels. */
#define hash_min(val, bits)							\
	(sizeof(val) <= 4 ? hash_32(val, bits) : hash_long(val, bits))
#endif // // hash function

#endif