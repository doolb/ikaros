CC=clang
IDIR =src
CFLAGS=-I $(IDIR) -I 3rd/rpmalloc/rpmalloc  -I 3rd -I 3rd/glfw/include -I 3rd/mappedfile -lglfw3 -lGL -lX11 -lXi -lXrandr -lXxf86vm -lXinerama -lXcursor -lrt -lm -pthread
ODIR =obj
STDIR =./Test

mem: dir
	$(CC) -c 3rd/rpmalloc/rpmalloc/rpmalloc.c -o $(ODIR)/$@.o
mmap: dir
	$(CC) -c 3rd/mappedfile/mappedfile.c -o $(ODIR)/$@.o
glad: dir
	$(CC) -c 3rd/glad/glad.c -o $(ODIR)/$@.o -I 3rd

test: glad mem mmap
	$(CC) $(STDIR)/test.c -I $(STDIR)  $(STDIR)/func/*.c $(ODIR)/*.o -o bin -ldl -rdynamic -fvisibility=hidden $(CFLAGS) -g

clean:
	rm -f bin $(ODIR)/*
dir:
	mkdir -p $(ODIR)
