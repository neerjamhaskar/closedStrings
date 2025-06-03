#include "USet.h"
#include <iostream>
#include <algorithm>

// Constructor to initialize from a vector
USet::USet(const int& data, const int& lcp){
    this->data.insert(data);
    this->LCP = lcp;
}

// Insert all elements from another USet into this USet
void USet::MultiInsert(const USet& other) {
    data.insert(std::make_move_iterator(other.data.begin()), std::make_move_iterator(other.data.end()));
}

// Find the maximum element in `other` less than each element in this set and return as pairs
std::vector<std::pair<int, int>> USet::MultiPredecessor(const USet& other) const {
    std::vector<std::pair<int, int>> result;
    for (int x : data) {
        auto it = other.data.lower_bound(x);
        if (it != other.data.begin()) {
            --it;
            result.emplace_back(x, *it);
        }
        // Ignore cases where no valid predecessor is found
    }
    return result;
}

// Find the minimum element in `other` greater than each element in this set and return as pairs
std::vector<std::pair<int, int>> USet::MultiSuccessor(const USet& other) const {
    std::vector<std::pair<int, int>> result;
    for (int x : data) {
        auto it = other.data.upper_bound(x);
        if (it != other.data.end()) {
            result.emplace_back(x, *it);
        }
        // Ignore cases where no valid successor is found
    }
    return result;
}

// Utility function to insert a single element
void USet::Insert(int element) {
    data.insert(element);
}

// Print the elements of the set
void USet::Print() const {
    if (data.empty()) {
        std::cout << "USet is empty" << std::endl;
    } else {
        std::cout << "USet elements: ";
        for (int el : data) {
            std::cout << el << " ";
        }
        std::cout << std::endl;
    }
}

// Global next array
std::vector<int> next;

std::pair<std::vector<std::pair<int, int>>, USet*> Union(std::vector<USet*>& sets) {
    if (sets.empty()) {
        return {{}, nullptr}; // Return empty results if no valid sets
    }

    // Step 1: Find the largest set and store it in largestSet
    USet* largestSet = sets[0];

    for (USet* set : sets) {
        if (set->LCP > 0 && set->data.size() > largestSet->data.size()) {
            largestSet = set; // Update largestSet if we find a larger one
        }
    }

    // Step 2: Perform largestSet->MultiInsert on all remaining sets
    for (USet* set : sets) {
        if (set != largestSet) {
            largestSet->MultiInsert(*set);
        }
    }

    // Step 3: Initialize CL as an empty list
    std::vector<std::pair<int, int>> CL;

    // Step 4: Iterate through each remaining set and collect MultiSuccessor and MultiPredecessor results
    for (USet* set : sets) {
        if (set != largestSet) {
            // Collect MultiSuccessor and MultiPredecessor
            auto successors = set->MultiSuccessor(*largestSet);
            auto predecessors = set->MultiPredecessor(*largestSet);
            
            // Push back only if successors are not empty and check `next` condition
            if (!successors.empty()) {
                for (const auto& [x, y] : successors) {
                    if (next[x] != y) {
                        CL.emplace_back(x, y);
                        next[x] = y; // Update next[x] to y
                    }
                }
            }

            // Push back only if predecessors are not empty and check `next` condition
            if (!predecessors.empty()) {
                for (const auto& [y, x] : predecessors) {
                    if (next[x] != y) {
                        CL.emplace_back(x, y);
                        next[x] = y; // Update next[x] to y
                    }
                }
            }
            // Clear up sets that are no longer used
            delete set;
        }
    }


    // Return CL and a pointer to the largest set
    return {std::move(CL), largestSet};
}
