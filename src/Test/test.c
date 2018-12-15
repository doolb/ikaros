#include "test.h"
#include "System.h"

#define FUNC_TEST "_test"

char buff [256] = { 0 };

int main (int argc, char **argv) {

	_test_fun f = NULL;
	char *file = argc <= 2 ? NULL : argv [2];
	char *name = argc <= 1 ? NULL : argv [1];
	printf ("Args: [func=%s] [dll=%s]\n", name == NULL ? "(all)" : name,
		file == NULL ? argv [0] : file);

	void *m = getModule (file);
	if (!m) {
		printf ("Load file failed. %s", file);
		return -1;
	}

	if (name != NULL) {
		// run specify
		snprintf (buff, 255, "%s" FUNC_TEST, name); // insert a "_test"
		f = (_test_fun) getProc (m, buff);
		if (f)
			f ();
		else
			printf ("No test function found!\n");
	}
	else {
#if _WIN32
		int i = 1;
		do {
			f = (_test_fun) getProc (m, i);
			if (f) {
				printf ("%d ", i++);
				f ();
			}
			putchar ('\n');
		} while (f != NULL);
#elif __linux__
		// find all *_test symbol
		snprintf (buff, 255,
			"nm %s -D | grep %s |while read ptr type name; do printf "
			"\"%%s\\n\" ${name}; done",
			file == NULL ? argv [0] : file, FUNC_TEST);
		FILE *fn = popen (buff, "r");
		while (!feof (fn)) {
			memset (buff, 0, sizeof (buff));
			if (fgets (buff, 255, fn) > 0 && buff [0] != '\n') {
				buff [strlen (buff) - 1] = '\0'; // remove the last \n
				f = (test_fun) getProc (m, buff);
				if (f)
					f ();
				else
					printf ("%s: no found!\n", buff);
				putchar ('\n');
			}
		}
		pclose (fn);
#endif
	}

	printf ("End.\n");

	return 0;
}

test_start (test2) { test_func ("test %x", 1, 123); }
test_end ()