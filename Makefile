CXX = g++
CXXFLAGS = -std=c++20

objects = main.o intel_8080.o instructions.o memory.o argument_parser.o
test_objects = intel_8080.o instructions.o memory.o

all: test_runner main.out clean

main.out: $(objects)
	$(CXX) $^ -o $@

test_runner: $(test_objects) test.o
	$(CXX) $^ -o $@
	@echo "Running build tests."
	@./test_runner || (echo "Tests failed. Run ./test_runner manually with debugging enabled." && exit 1)
	@echo "Build tests successful."

# Build rule for src/*.cpp files
%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build rule for src/argument_parsing/*.cpp files
%.o: src/argument_parsing/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test.o: test/test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o src/argument_parsing/*.o *.o test_runner
