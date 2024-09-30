CXX = clang++
CXXFLAGS += -Xclang -fopenmp -L/opt/homebrew/opt/libomp/lib\
	 -I/opt/homebrew/opt/libomp/include -lomp -std=c++11
# Debug flags
CXXFLAGS_DEBUG = -g -O0 -DDEBUG

# Release flags
CXXFLAGS_RELEASE = -O3

# Default to debug build
CXXFLAGS += $(CXXFLAGS_DEBUG)

# Target for debug build
debug: CXXFLAGS += $(CXXFLAGS_DEBUG)
debug: ex1 ex2 ex3

# Target for release build
release: CXXFLAGS += $(CXXFLAGS_RELEASE)
release: debug

ex1:
	$(CXX) $(CXXFLAGS) ex1.cpp -o build/ex1
ex2:
	$(CXX) $(CXXFLAGS) ex2.cpp -o build/ex2
ex3:
	$(CXX) $(CXXFLAGS) ex3.cpp -o build/ex3
clean:
	rm -f build/ex1 build/ex2 build/ex3
run:
	./build/ex1
	./build/ex2
	./build/ex3