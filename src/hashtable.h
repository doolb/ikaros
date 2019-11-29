#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <assert.h>
#include "config.h"
#include "log2.h"
#include "System.h"

#define HASH_EXPAND 0.72

// hashtable implements
// refer:https://kernelnewbies.org/FAQ/Hashtables
// insert the struct in you own data-struct
typedef struct _HashList {
	// key
	void* key;
	size_t key_byte; // if key_byte is zero, just compare key address as integer

	int hashCode;
	
	struct _HashList *next; // point to next entry
}HashList, *pHashList;

typedef struct _HashTable {
	IArray(pHashList)
#ifdef TRACE_HASH_CONFLICATED
	unsigned char* item_count;
	int max_conflicated;
#endif
}HashTable, *pHashTable;

// define a HashList in you own struct
#define Def_HashList    HashList _list_
#define Def_HashList_Init ._list_ = {0}



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

static inline uint hash_32(uint val, uint bits)
{
	/* On some cpus multiply is faster, on others gcc will do shifts */
	uint hash = val * GOLDEN_RATIO_PRIME_32;

	/* High bits are more random, so use them. */
	return hash >> (32 - bits);
}
static inline ulong hash_64(ulong val, uint bits)
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

#define hash_str	hash_djb2_s
#define hash_str_bit(str, max_len, bits) (hash_str(str, max_len) & pow2_ff(bits))

// hash for string, also for any object
// refer:http://www.cse.yorku.ca/~oz/hash.html
static inline unsigned long hash_djb2_s(char *str, int max_len) {
	unsigned long hash = 5381;
	for (int i = 0; i < max_len && str[i]; i++)
		hash = ((hash << 5) + hash) + str[i];
	return hash;
}

/* Use hash_32 when possible to allow for fast 32bit hashing in 64bit kernels. */
#define hash_min(val, bits)							\
	(sizeof(val) <= 4 ? hash_32(val, bits) : hash_long(val, bits))
#endif // // hash function

static inline void hashNew(pHashTable table) {
	IArray_New(table);
#ifdef TRACE_HASH_CONFLICATED
	table->item_count = NULL;
	table->max_conflicated = 0;
#endif
}

static inline void hashDel(pHashTable table) {
	IArray_Del(table, { mfree(table->items); });
#ifdef TRACE_HASH_CONFLICATED
	mfree(table->item_count);
	table->max_conflicated = 0;
#endif
}

static inline int hashInit(pHashTable table) {
	int prime = GetPrime(table->count);
	void* mem;
	void* mem_count;
	if (table->items != NULL) {
		mem = mralloc(table->items, prime * sizeof(pHashTable));
#ifdef TRACE_HASH_CONFLICATED
		mem_count = mralloc(table->item_count, prime * sizeof(int));
#endif
	}
		
	else {
		mem = mcalloc(prime, sizeof(pHashTable));
#ifdef TRACE_HASH_CONFLICATED
		mem_count = mcalloc(prime, sizeof(int));
#endif
	}
		
	if (mem == NULL) {
		return -1;
	}
#ifdef TRACE_HASH_CONFLICATED
	if (mem_count == NULL) {
		return -1;
	}
#endif
	table->max = prime;
	table->items = mem;
#ifdef TRACE_HASH_CONFLICATED
	table->item_count = mem_count;
	table->max_conflicated = 0;
#endif
	return prime;
}

static inline _hashInsert(pHashTable table, int hash, HashList *value) {
	int idx = hash % table->max;
	pHashList* phash = &(table->items[idx]);
	while (*phash != NULL) {
		if(((*phash)->next == NULL))
			break;
		phash = &((*phash)->next);
	}
	if (*phash == NULL) {
		(*phash) = value;
		_debug_(" %d %d @% d", table->count+1, value->key, idx);
	}
	else {
		(*phash)->next = value;
		_debug_("conflicted %d %d @ %d", table->count + 1, value->key, idx);
#ifdef TRACE_HASH_CONFLICATED
		table->item_count[idx]++;
		table->max_conflicated++;
#endif
	}
	value->hashCode = hash;
	value->next = NULL;
	table->count++;
}
#define __hash__(key, key_byte) key_byte > 0 ? hash_str(key, key_byte) : hash_min(key, 31)
static inline int hashResize(pHashTable table, int newSize, bool forceNewHashCode) {
	_info_("(%d/%d) -> %d", table->count, table->max, newSize);
 	pHashList* newTables = mcalloc(newSize, sizeof(pHashList));
	if (newTables == NULL)
		return -1;

#ifdef TRACE_HASH_CONFLICATED
	mfree(table->item_count);
	table->item_count = mcalloc(newSize, sizeof(int));
	if (table->item_count == NULL)
		return -1;
	table->max_conflicated = 0;
#endif

	pHashList* oldTables = table->items;
	int oldMax = table->max;
	table->items = newTables;
	table->max = newSize;
	table->count = 0;

	for (int i = 0; i < oldMax; i++) {
		pHashList ptr = oldTables[i];
		while (ptr != NULL)
		{
			int newHash = forceNewHashCode ? __hash__(ptr->key, ptr->key_byte) : ptr->hashCode;
			pHashList next = ptr->next;
			ptr->next = NULL;
			_hashInsert(table, newHash, ptr);
			ptr = next;
		}
	}
	mfree((oldTables));
	oldTables = NULL;

	return newSize;
}

static inline bool is_hash_same(pHashList ptr, int hash, void* key, int key_byte) {
	if (ptr->hashCode == hash && ptr->key_byte == key_byte) {
		if (key_byte > 0 && memcmp(key, ptr->key, key_byte))
			return true;
		else if (key == ptr->key)
			return true;
	}
	return false;
}

// if key_byte is zero, just compare key address as integer
static inline bool hashAdd(pHashTable table, void* key, int key_byte, HashList *value) {
	if (table->items == NULL)
		hashInit(table);
	int hash = __hash__(key, key_byte);
	int idx = hash % table->max;
	
	// is exisit
	pHashList ptr = table->items[idx];
	while (ptr != NULL) {
		if(is_hash_same(ptr, hash, key, key_byte))
			return false;
		ptr = ptr->next;
	}

	// is need resize
	if (table->count >= table->max/*(int)(table->max * HASH_EXPAND)*/)
		if( hashResize(table, ExpandPrime(table->max), false) < 0)
			return false;

	// insert
	value->key = key;
	value->key_byte = key_byte;
	_hashInsert(table, hash, value);
	return true;
}

static void* hashGet(pHashTable table, void* key, int key_byte) {
	if (table->items == NULL || table->count == 0)
		return NULL;
	int hash = __hash__(key, key_byte);
	int idx = hash % table->max;

	pHashList ptr = table->items[idx];
	while (ptr != NULL) {
		if (is_hash_same(ptr, hash, key, key_byte))
			return ptr;
		ptr = ptr->next;
	}
	return ptr;
}


_Func(HashTable) {
	IDispose(HashTable);
	ITable(HashTable, HashList);
} static _(HashTable) = {
	IDispose_Def(hashNew, hashDel, hashInit),
	ITable_Def(hashAdd, hashGet)
};

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
	_hlistAdd (&((node)->_list_), &(*hashtable) [hash_min(key,Hash_Bits(hashtable))])

#define HS_LEN 255
// add a node define with `Def_HashList` to hash-table, the key is string or any object
#define hashAdds(hashtable, node, key, max_len)		\
	_hlistAdd (&((node)->_list_), &(*hashtable) [hash_str_bit (key, max_len, Hash_Bits (hashtable))])

// add node to hash-list head
static inline void _hlistAdd (pHashList node, pHashList head) {
	assert (node != head);
	node->next = head->next;
	head->next = node;
}

#define Hash_for_each(table, bkt, Type, obj, block)						\
	do{																	\
		for((bkt)=0; (bkt) < (table)->max;	(bkt)++) {					\
			if ((table)->items[bkt] == NULL)	continue;					\
			for (obj = containof(a.items[bkt], mystruct, _list_);		\
				;														\
				obj = containof(cur->_list_.next, mystruct, _list_)) {	\
				block													\
				if ((obj->_list_.next) == NULL) break;					\
			}															\
		}																\
	}while(0)															\


// iterate in a hash-list
#define Hlist_for_each_entry(Type,pos,head)							\
    for(pos = List_Entry((head)->next, Type, _list_);				\
        (&pos->_list_.next) != NULL;								\
        pos = List_Entry(pos->_list_.next, Type, _list_))

// iterate each entry in hash-table
#define Hash_for_each_entry(table, bkt, Type, obj)	\
	for((bkt)=0; (bkt) < (table)->max;	(bkt)++)\
		Hlist_for_each_entry(Type, obj, &((table)->items[bkt]))

// access entry in hash-table

static inline void _hlistRemove (pHashList node, pHashList head) {
	assert (node != head);

}

#endif