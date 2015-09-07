CXX=g++
CXXFLAGS=-std=c++0x -c -g -Wall -pedantic -O2
LDFLAGS=
SOURCES=main.cpp GA.cpp FLM_Conf.cpp GA_Conf.cpp util.cpp Fitness.cpp Representation/BackoffGraph.cpp Representation/Gene.cpp Representation/Node.cpp GA_Operator/Crossover.cpp GA_Operator/GA_Operator.cpp GA_Operator/Initializator.cpp GA_Operator/Mutation.cpp GA_Operator/Selection.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=ga-flm

.PHONY: all clean

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -v -f *.o GA_Operator/*.o Representation/*.o
