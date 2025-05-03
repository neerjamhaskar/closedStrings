#include <bits/stdc++.h>
#include <fstream>
#define MAX_LENGTH 10000

// Suffix struct to store suffixes and their starting indices.
struct Suffix {
    uint32_t index;
    uint32_t lcp;
    char* suffix;
};

// Simple suffixComparator
bool suffixComparator(const Suffix& a, const Suffix& b) {
    return strcmp(a.suffix, b.suffix) < 0;
}

// Function to compute the suffix array for a given text
std::vector<Suffix> computeSuffixArray(char* text) {
    uint32_t n = std::strlen(text);
    std::vector<Suffix> suffixArray(n);

    for(uint32_t i = 0; i < n; ++i) {
        suffixArray[i].index = i;
        suffixArray[i].suffix = text + i;
    }

    std::sort(suffixArray.begin(), suffixArray.end(), suffixComparator);
    return suffixArray;
}

// Kasai's Algorithm to compute the LCP array
void computeLCPArray(std::vector<Suffix>& suffixArray, const char* const text) 
{
    uint32_t n = suffixArray.size();
    std::vector<uint32_t> inverseSuffixArray(n);

    for(size_t i = 0; i < n; ++i)
        inverseSuffixArray[suffixArray[i].index] = i;

    int32_t clcp = 0;
    for(size_t i = 0; i < n; ++i)
    {
        if(inverseSuffixArray[i] == 0) 
        {
            suffixArray[inverseSuffixArray[i]].lcp = 0;
            clcp = 0;
            continue;
        }
        uint32_t j = suffixArray[inverseSuffixArray[i] - 1].index;
        while(i + clcp < n && j + clcp < n && text[i + clcp] == text[j + clcp])
            ++clcp;
        suffixArray[inverseSuffixArray[i]].lcp = clcp;

        // For next clcp we will do 
        clcp = std::max(clcp - 1, 0);
    }
}


// Function to write the string, SA, and LCP arrays in binary format
void writeBinaryFiles(const std::vector<Suffix>& suffixArray, const std::string& filename, const char* text) {
    // Write original text to the main file
    std::ofstream file(filename, std::ios::binary);
    file.write(text, std::strlen(text));
    file.close();

    // Write SA to filename.sa in binary
    std::ofstream saFile(filename + ".sa", std::ios::binary);
    for (const Suffix& suffix : suffixArray) {
        int index = suffix.index;
        saFile.write(reinterpret_cast<const char*>(&index), sizeof(index));
    }
    saFile.close();

    // Write LCP to filename.lcp in binary
    std::ofstream lcpFile(filename + ".lcp", std::ios::binary);
    for (const Suffix& suffix : suffixArray) {
        int lcp = suffix.lcp;
        lcpFile.write(reinterpret_cast<const char*>(&lcp), sizeof(lcp));
    }
    lcpFile.close();
}

int main() {
    std::unique_ptr<char[]> text(new char[MAX_LENGTH]);
    std::cout << "Enter the string to compute SA and LCP: ";
    std::cin >> std::setw(MAX_LENGTH) >> text.get();

    std::string filename;
    std::cout << "Enter the base filename: ";
    std::cin >> filename;

    // Compute SA and LCP
    std::vector<Suffix> suffixLCPArray = computeSuffixArray(text.get());
    computeLCPArray(suffixLCPArray, text.get());

    // Write original string, SA, and LCP to binary files
    writeBinaryFiles(suffixLCPArray, filename, text.get());

    std::cout << "String, SA, and LCP array written to " 
              << filename << ", " << filename << ".sa, and " << filename << ".lcp" << std::endl;
    return 0;
}
