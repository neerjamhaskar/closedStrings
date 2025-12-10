CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wno-unused-result

# Target to build the executable
all: closedStringsCrochemore

# Object file for the closedStrings application
closedStringsCrochemore.o: croch10.cpp
	$(CXX) $(CXXFLAGS) -c croch10.cpp

# Link object files to create the final executable
closedStringsCrochemore: closedStringsCrochemore.o
	$(CXX) $(CXXFLAGS) -o closedStringsCrochemore croch10.o

# Clean up object files and the executable
clean:
	rm -f *.o closedStringsCrochemore