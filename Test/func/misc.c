#include "test.h"
#include "config.h"
#include "mappedfile.h"

struct icontain {
	int idata;
};

struct mycontain {
	int data;
	int data2;
	struct icontain idata;
};

test_start (contain) {
	struct mycontain c = null;
	printf ("struct : \t%p\n", &c);
	printf ("struct data: \t%p\n", &(c.data));
	printf ("struct data2: \t%p\n", &(c.data2));
	printf ("struct idata: \t%p\n", &(c.idata));

	struct icontain *idata = &c.idata;
	printf ("offset of icontain: %p %zu\n", idata, offsetof (struct mycontain, idata));
	struct mycontain *h = containof (idata, struct mycontain, idata);
	printf ("get my contain: %p\n", h);
}
test_end ()

void mycontain_add (struct mycontain *obj, int a) {
	obj->data += a;
}
struct _mycontain_Func {
	void (*add)(struct mycontain *obj, int a);
}mycontain_Func = {
	.add = mycontain_add,
};

test_start (func) {
	struct mycontain c = null;
	c.data = 2;
	_ (mycontain).add (&c, 3);
	mycontain_Func.add (&c, 3);

	printf ("data: %d\n", c.data);
}
test_end ()


test_start (mmap) {
	size_t size = 0;
	char* file = __FILE__;
	char* data = map_file (file, &size);
	test_func ("open memory map file. %s , %zu", data, file, size);

	fprintf (stdout, "%s\n", data);

	unmap_file (data, size);
}
test_end()