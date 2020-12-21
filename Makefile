EXENAME = main
OBJS = main.o graph.o heap.o airport.o route.o
TEST_OBJS = graph.o heap.o airport.o route.o

CXX = clang++
CXXFLAGS = -std=c++1y -stdlib=libc++ -c -g -O0 -Wall -Wextra -pedantic
LDFLAGS = -std=c++1y -stdlib=libc++ -lc++abi -lm

all : $(EXENAME)

$(EXENAME) : $(OBJS) 
	$(CXX) $(OBJS) $(LDFLAGS) -o $(EXENAME)

main.o : main.cpp graph.h
	$(CXX) $(CXXFLAGS) main.cpp

graph.o : graph.cpp heap.h airport.h route.h
	$(CXX) $(CXXFLAGS) graph.cpp

heap.o : heap.cpp
	$(CXX) $(CXXFLAGS) heap.cpp

airport.o : airport.cpp route.h
	$(CXX) $(CXXFLAGS) airport.cpp

route.o : route.cpp airport.h
	$(CXX) $(CXXFLAGS) route.cpp

test : tests.o $(TEST_OBJS)
	$(CXX) tests.o $(TEST_OBJS) $(LDFLAGS) -o test

tests.o : tests/tests.cpp tests/catch.hpp graph.h
	$(CXX) $(CXXFLAGS) tests/tests.cpp

clean:
	-rm -f *.o $(EXENAME) test