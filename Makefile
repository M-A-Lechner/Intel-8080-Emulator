objects = main.o intel_8080.o instructions.o
all: $(objects)
	$(CXX) $^ -o a.out

$(objects): %.o: %.cpp
	$(CXX) -c src/$^ -o $@

%.cpp:
	touch src/$@

clean:
	rm -f *.cpp *.o