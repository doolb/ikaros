#include "test.h"
#include "../System/System.h"

int main(int argc, char** argv) {
	if (argc == 1) {
		printf("Args: [dll] func/'*'");
		return 1;
	}

	test_fun f = NULL;

	char* name = argc == 2 ? argv[1] : argv[2];
	ptr m = argc == 2 ? getModule(NULL) : loadModule(argv[1]);
	if (!m) {
		printf("Load file failed. %s", argv[1]);
		return -1;
	}
	if (name[0] != '*') {
		// run specify
		f = (test_fun)getProc(m, name);
		if (f) f();
		else printf("No test function found!");
	}
	else {
		int i = 1;
		do {
			f = (test_fun)getProc(m, i);
			if (f) { printf("%d ", i++); f(); }
		} while (f != NULL);
	}

	printf("End.\n");
	_getch();
	return 0;
}