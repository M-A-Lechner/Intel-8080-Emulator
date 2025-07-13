objects = main.o intel_8080.o instructions.o memory.o
all: $(objects)
	$(CXX) $^ -o a.out

$(objects): %.o: %.cpp
	$(CXX) -std=c++20 -c src/$^ -o $@

%.cpp:
	touch src/$@

clean:
	rm -f *.cpp *.o