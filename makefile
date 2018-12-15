CC=clang
IDIR =src/PCL/inc
CFLAGS=-I $(IDIR) -I 3rd/rpmalloc/rpmalloc
ODIR =.
SDIR =src

3RD = 3rd/rpmalloc/rpmalloc/rpmalloc.c

_3rd:
	$(CC) $(3RD) -o $(ODIR)/3rd.o -c

test: _3rd
	$(CC) $(SDIR)/Test/*.c 3rd.o -o $(ODIR)/bin -ldl -rdynamic -fvisibility=hidden $(CFLAGS) -g

clean:
	rm -f $(ODIR)/bin $(ODIR)/3rd.o