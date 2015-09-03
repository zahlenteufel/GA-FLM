CXX=g++
CXXFLAGS=-std=c++0x -c -g -Wall -pedantic -O2
LDFLAGS=
SOURCES=main.cpp BackoffGraph.cpp FLM_Conf.cpp GA_Conf.cpp Gene.cpp Node.cpp ga-flm.cpp ga-operator.cpp util.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ga-flm

.PHONY: all clean

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -v -f *.o $(EXECUTABLE)
