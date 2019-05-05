#ifndef LIST_H
#define LIST_H

#include "config.h"
#include <assert.h>

// doubly linked list implementation
// refer: https://kernelnewbies.org/FAQ/LinkedLists
// insert the struct in you own data-struct
typedef struct _ListHead {
	struct _ListHead *next; // point the next entry
	struct _ListHead *prev; // point the previous entry
}ListHead, *pListHead;

#define Def_ListHead    ListHead _list_;

// init the list head when define
#define List_Init(name)			{&(name), &(name)}
// init the list head define with `Def_ListHead`
#define Def_ListHead_Init(name)		._list_ = List_Init(name._list_)

// init a list head define with `Def_ListHead`
#define listInit(ptr) _listInit(&((ptr)->_list_))
// init the list head runtime
static inline void _listInit (pListHead list) {
	list->next = list;
	list->prev = list;
}

static bool isListEmpty(pListHead list) {
	return list->next == list->prev && list->next == list;
}

// define a list head and init it
#define List_Head(name) ListHead name = List_Init(name)

// add a entry to list head position
#define listAdd(ptr,head)	_listAdd (&((ptr)->_list_), (head)->prev, head)
// internal function for add list
static inline void _listAdd (pListHead _new, pListHead prev, pListHead next) {
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}


#define listRemove(ptr) _listRemove(&((ptr)->_list_))
static inline void _listRemove (pListHead entry) {
	// this entry must no be head
	assert (entry->next != entry->prev);
	entry->next->prev = entry->prev;
	entry->prev->next = entry->next;
	entry->prev = entry->next = entry; // point to it's self
}


// iterate over a list head
#define List_for_each(pos, head)    \
    for( pos = (head)->next; pos != (head); pos = pos->next)

// iterate over list of given type
// the struct must define within a Def_ListHead
// only gcc and clang support `typeof` keyword
#define List_for_each_entry(Type,pos,head)							\
    for(pos = List_Entry((head)->next, Type, _list_);				\
        &pos->_list_ != (head);										\
        pos = List_Entry(pos->_list_.next, Type, _list_))
#endif