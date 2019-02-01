#include "test.h"
#include "list.h"

typedef struct _mystruct {
	int data;
	Def_ListHead
}mystruct;

test_start (list) {
	List_Head (head);

	mystruct first = {
		.data = 10,
		Def_ListHead_Init (first)
	};
	mystruct second;
	second.data = 20;
	listInit (&second);

	listAdd (&first, &head);
	listAdd (&second, &head);

	printf ("link head: addr %p, prev = %p, next = %p\n", &head, head.prev, head.next);
	pListHead pos = NULL;
	mystruct * ptr = NULL;
	List_for_each (pos, &head) {
		printf ("link list: addr %p, prev = %p, next = %p\n",
			pos,
			pos->prev,
			pos->next);
		ptr = List_Entry (pos, mystruct, _list_);
		printf ("\tdata: %p %d\n", ptr, ptr->data);
	}

	listRemove (&first);
	listAdd (&first, &head);

	printf ("itera with List_for_each_entry\n");
	List_for_each_entry (mystruct, ptr, &head) {
		printf ("entry : %p %d\n", ptr, ptr->data);
	}
}
test_end ()