#ifndef USET_H
#define USET_H

#include <set>
#include <vector>
#include <utility> // For std::pair
#include <vector>
#include <list>
#include <cstddef>
#include <cstdint>

class USet {
public:
    std::set<int> data; // Ordered set of unique integers
    uint32_t LCP;

    // Constructors
    USet() = default;
    USet(const int& data, const int& lcp);

    // Methods
    void MultiInsert(const USet& other);
    std::vector<std::pair<int, int>> MultiPredecessor(const USet& other) const;
    std::vector<std::pair<int, int>> MultiSuccessor(const USet& other) const;

    // Utility to add a single element
    void Insert(int element);
    // Utility to print the elements of the set
    void Print() const;
};


// Function that returns the combined list (CL) and a pointer to the largest set
std::pair<std::vector<std::pair<int, int>>, USet*> Union(std::vector<USet*>& sets);

#endif // USET_H
