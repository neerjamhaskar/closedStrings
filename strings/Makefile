# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++17 -O3
# Target executable name
TARGET = smallStringArrayGenerator

# Default target to compile the code
all: $(TARGET)

# Rule to build the target executable
$(TARGET): smallStringArrayGenerator.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) smallStringArrayGenerator.cpp

# Rule to clean up generated files
clean:
	rm -f $(TARGET) 
# Phony targets
.PHONY: all clean
