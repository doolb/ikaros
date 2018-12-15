CC=clang
IDIR =src/PCL/inc
CFLAGS=-I $(IDIR) -I 3rd/rpmalloc/rpmalloc
ODIR =.
SDIR =src

test: 
	$(CC) $(SDIR)/Test/*.c -o $(ODIR)/bin -ldl -rdynamic -fvisibility=hidden $(CFLAGS) -g

clean:
	rm -rf $(ODIR)/*