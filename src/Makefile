CXX = g++
CXXFLAGS = -std=c++17 -Wall -O3 -Wno-unused-result

# Target to build the executable
all: closedStrings

# Object file for USet
USet.o: USet.cpp
	$(CXX) $(CXXFLAGS) -c USet.cpp

# Object file for the closedStrings application
closedStrings.o: closedStrings.cpp USet.h 
	$(CXX) $(CXXFLAGS) -c closedStrings.cpp

# Link object files to create the final executable
closedStrings: closedStrings.o USet.o
	$(CXX) $(CXXFLAGS) -o closedStrings closedStrings.o USet.o

# Clean up object files and the executable
clean:
	rm -f *.o closedStrings