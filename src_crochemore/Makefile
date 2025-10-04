CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wno-unused-result

# Target to build the executable
all: closedStrings

# Object file for the closedStrings application
closedStrings.o: croch10.cpp
	$(CXX) $(CXXFLAGS) -c croch10.cpp

# Link object files to create the final executable
closedStrings: closedStrings.o
	$(CXX) $(CXXFLAGS) -o closedStrings croch10.o

# Clean up object files and the executable
clean:
	rm -f *.o closedStrings