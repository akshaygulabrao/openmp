CC = clang
CFLAGS += -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib\
	 -I/opt/homebrew/opt/libomp/include -lomp -lstdc++ -std=c++11
# Debug flags
CFLAGS_DEBUG = -g -O0 -DDEBUG

# Release flags
CFLAGS_RELEASE = -O3

# Default to debug build
CFLAGS += $(CFLAGS_DEBUG)

# Target for debug build
debug: CFLAGS += $(CFLAGS_DEBUG)
debug: ex1 ex2 test

# Target for release build
release: CFLAGS += $(CFLAGS_RELEASE)
release: debug


ex1:
	$(CC) $(CFLAGS) ex1.cpp -o build/ex1
ex2:
	$(CC) $(CFLAGS) ex2.cpp -o build/ex2
test:
	$(CC) $(CFLAGS) test.cpp -o build/test
clean:
	rm -f build/ex1 build/ex2 build/test
run:
	./build/ex1
	./build/ex2