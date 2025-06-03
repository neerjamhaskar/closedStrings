#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <unordered_map>
#include <stdio.h>
#include <stdlib.h>

#include "USet.h"

extern std::vector<int> next;

void computeClosedStrings(std::vector<std::vector<std::pair<uint64_t, uint64_t>>>& MRC, const uint64_t& n, const unsigned char* W)
{
    printf("The Compact Representation of all Closed Strings of string %s : \n", W);
    for(uint64_t i = 0; i < n; ++i)
    {
        uint64_t n_rcs = MRC[i].size(); // Number of right closed strings at i
        for(uint64_t j = 0; j < n_rcs; ++j)
        {
            uint64_t stringLength = MRC[i][j].first;
            uint64_t borderLength = MRC[i][j].second;
            std::string rightClosedString(W + i, W + i + stringLength);
            uint64_t nextRCSBorderLength;

            if(j == n_rcs - 1) // stringLength - 1 == borderLength
                printf("%s : (%ld, %ld, %ld)\n", rightClosedString.c_str(), i, 1ul, stringLength);
            else
            {
                nextRCSBorderLength = MRC[i][j + 1].second;
                printf("%s : (%ld, %ld, %ld)\n", rightClosedString.c_str(), i, (stringLength + nextRCSBorderLength - borderLength + 1), stringLength);
            }
        }
    }
}

void computeMaximalClosedStrings(const std::vector<std::vector<std::pair<uint64_t, uint64_t>>>& MRC, const uint64_t& n, const unsigned char* W)
{
    printf("MCS List for the string %s : \n", W);
    for(uint64_t i = 0; i < n; ++i)
    {
        uint64_t n_rcs = MRC[i].size(); // Number of right closed strings at i
        for(uint64_t j = 0; j < n_rcs; ++j)
        {
            uint64_t stringLength = MRC[i][j].first;
            uint64_t borderLength = MRC[i][j].second;
            std::string MaximalClosedString(W + i, W + i + stringLength);

            if(i == 0 || W[i - 1] != W[i + stringLength - borderLength - 1])
                printf("%s, ", MaximalClosedString.c_str());
                
        }
    }
    printf("\n");
}

int main(int argc, char **argv) 
{
    if (argc != 3 || (std::string(argv[2]) != "mcs" && std::string(argv[2]) != "cs")) {
        fprintf(stderr, "Usage: ./closedStrings <text_file without extension> [cs|mcs]\n");
        return -1;
    }

    uint64_t n;
    uint32_t *SA = nullptr;
    uint32_t *LCP = nullptr;
    unsigned char *W = nullptr;

    // File pointers for .sa and .lcp files
    FILE *fp = nullptr;
    FILE *fp_sa = nullptr;
    FILE *fp_lcp = nullptr;

    // Create the filenames with extensions
    std::string sa_filename = std::string(argv[1]) + ".sa";
    std::string lcp_filename = std::string(argv[1]) + ".lcp";

        // Open .sa file and read into W array
    fp = fopen(argv[1], "rb");
    if (fp == nullptr) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return -1;
    }

    // Get the number of integers in the .sa file
    fseek(fp, 0L, SEEK_END);
    n = ftell(fp);  // Each char is 1
    rewind(fp);

    // Allocate memory for W array
    W = (unsigned char *)malloc(sizeof(unsigned char) * (n));
    if (W == nullptr) {
        fprintf(stderr, "Memory allocation failed for W\n");
        fclose(fp);
        return -1;
    }

    // Read text file into W array
    fread(W, sizeof(unsigned char), n, fp);
    fclose(fp);

    // Open .sa file and read into SA array
    fp_sa = fopen(sa_filename.c_str(), "rb");
    if (fp_sa == nullptr) {
        fprintf(stderr, "Error: Could not open file %s\n", sa_filename.c_str());
        return -1;
    }

    // Get the number of integers in the .sa file
    fseek(fp_sa, 0L, SEEK_END);
    n = ftell(fp_sa) / 4;  // Each int is 4 bytes
    rewind(fp_sa);

    // Allocate memory for SA array
    SA = (uint32_t *)malloc(sizeof(uint32_t) * (n));
    if (SA == nullptr) {
        fprintf(stderr, "Memory allocation failed for SA\n");
        fclose(fp_sa);
        return -1;
    }

    // Read .sa file into SA array
    fread(SA, sizeof(uint32_t), n, fp_sa);
    fclose(fp_sa);

    // Open .lcp file and read into LCP array
    fp_lcp = fopen(lcp_filename.c_str(), "rb");
    if (fp_lcp == nullptr) {
        fprintf(stderr, "Error: Could not open file %s\n", lcp_filename.c_str());
        free(SA); // Clean up allocated memory
        return -1;
    }

    // Allocate memory for LCP array (assuming same size as SA)
    LCP = (uint32_t *)malloc(sizeof(uint32_t) * (n));
    if (LCP == nullptr) {
        fprintf(stderr, "Memory allocation failed for LCP\n");
        fclose(fp_lcp);
        free(SA); // Clean up allocated memory
        return -1;
    }

    // Read .lcp file into LCP array
    fread(LCP, sizeof(uint32_t), n, fp_lcp);
    fclose(fp_lcp);

    std::stack<USet*> S;

    S.push(new USet(SA[0], LCP[0])); 
    std::vector<std::vector<std::pair<uint64_t, uint64_t>>> MRC(n, std::vector<std::pair<uint64_t, uint64_t>>());

    next.resize(n, -1);
    uint64_t i = 1;
    while (!S.empty() || i < n) 
    {
        while (i < n && (S.empty() || LCP[i] >= S.top()->LCP)) 
        {
            USet* newSet = new USet(SA[i], LCP[i]); 
            S.push(newSet);
            ++i;
        }

        USet* peakPoint = S.top(); 
        S.pop();

        std::vector<USet*> setList = {peakPoint}; // Collect setList for union
        uint32_t peakLCP = peakPoint->LCP;

        while (!S.empty() && S.top()->LCP == peakLCP) 
        {
            setList.push_back(S.top());
            S.pop();
        }

        if (peakLCP != 0) 
        {
            uint32_t newLCP = S.top()->LCP;
            setList.push_back(S.top());
            S.pop();

            // Combine setList and find the largest set
            auto [combinedList, largestSet] = Union(setList);
            
            for (const auto& [x, y] : combinedList) 
            {
                {
                    uint64_t l = y + peakLCP - x; 
                    MRC[x].push_back({l, peakLCP});
                }
            }
            largestSet->LCP = newLCP;
            S.push(largestSet); 
        }
        else
        {
            for(auto& setPtr : setList)
                delete setPtr;
        }
    }

    // Cleanup: Delete dynamically allocated USet objects from stack
    while (!S.empty()) {
        delete S.top();
        S.pop();
    }

    for (uint64_t i = 0; i < n; ++i)
    {
        if ((i == 0 && ((n > 1 && W[i] != W[i + 1]) || (n == 1))) ||         // First character case
            (i == n - 1 && n > 1) ||     // Last character case
            (i > 0 && i < n - 1 && W[i] != W[i + 1])) // Middle characters case
        {
            MRC[i].push_back({1, 0});
        }
    }
    
    printf("The MRC Array for the string %s : \n", W);
    for(uint64_t i = 0; i < n; ++i)
    {
        std::cout << i << ": "; 
        for(uint64_t j = 0; j < MRC[i].size(); ++j)
        {
            printf("(%ld,%ld), ", MRC[i][j].first, MRC[i][j].second);
        }
        printf("\n");
    }

    if(std::string(argv[2]) == "mcs")
        computeMaximalClosedStrings(MRC, n, W);
    else if(std::string(argv[2]) == "cs")
        computeClosedStrings(MRC, n, W);

    free(LCP);
    free(W);
    free(SA);
    return 0;
}
