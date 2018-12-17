CC=clang
IDIR =src/PCL/inc
CFLAGS=-I $(IDIR) -I 3rd/rpmalloc/rpmalloc  -I 3rd -I 3rd/glfw/include -lglfw3 -lGLU -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
ODIR =obj
SDIR =src

mem: dir
	$(CC) -c 3rd/rpmalloc/rpmalloc/rpmalloc.c -o $(ODIR)/$@.o
glad: dir
	$(CC) -c 3rd/glad/glad.c -o $(ODIR)/$@.o

test: glad mem
	$(CC) $(SDIR)/Test/*.c $(ODIR)/*.o -o bin -ldl -rdynamic -fvisibility=hidden $(CFLAGS) -g

clean:
	rm -f bin $(ODIR)/*
dir:
	mkdir -p $(ODIR)