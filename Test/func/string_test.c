#include "test.h"
#include "System.h"

#define STR " \n   \n shader \"sbsdd\" { a b c}"

test_start(string) {
	int i = 0, l = 0, c = 0;
	int len = strlen(STR);
	int s, e;

	parseEmpty(STR, len, &i, &l, &c);
	test_func("parseEmpty idx", i == 7 && l == 2 && c == 1);

	parseWord(STR, len, &i, &l, &c, &s, &e);
	test_func("parseKeyword idx", i == 13 && l == 2 && c == 7 && s == 7 && e == 12);

	parseCfgString(STR, len, &i, &l, &c, &s, &e);
	test_func("parseKeyword idx", i == 21 && l == 2 && c == 15 && s == 15 && e == 19);

	parseSymbol(STR,len, &i, &l, &c, &s, &e, '{', '}');
	test_func("parseKeyword idx", i == 30 && l == 2 && c == 24 && s == 23 && e == 28);
}
test_end();