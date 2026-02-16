# Closed Strings 

This project computes the following within a given string. For each input string, the program computes:
- The **maximal right-closed array (MRC)** of a given string $w[1..n]$ in $\mathcal{O}(n \log n)$-time given its SA and LCP array and $\mathcal{O}(n)$-auxiliary space.
- The **maximal right-closed array (MRC)** of a given string $w[1..n]$ in $\mathcal{O}(n \log n)$-time with no pre-processing and $\Theta(n)$-auxiliary space.
- The **compact representation** for all closed substrings of a given string $w[1..n]$ in $\mathcal{O}(n \log n)$-time. 
- All **maximal closed substrings (MCSs)** of a given string $w[1..n]$ in $\mathcal{O}(n \log n)$-time.
- To cite this repo please cite the corresponding research papers: [Efficient Computation of Closed Substrings - SPIRE 2025
](https://doi.org/10.1007/978-3-032-05228-5_15) & [Efficient Algorithms to Compute Closed Substrings - Submitted to TOCS 2026](https://arxiv.org/abs/2506.06452)
- Note: The implementation of Crochemore's algorithm to compute repetitions is taken from [A Note on Crochemore's Repetitions Algorithm a Fast Space-Efficient Approach](https://www.stringology.org/event/2002/p5.html). 


## Table of Contents
1. [Project Structure](#project-structure)
2. [Installation and Compilation](#installation-and-compilation)
3. [Examples](#examples)
4. [Requirements](#requirements)


## Project Structure

- `src/` - Source code for computing all the above mentioned data structures and strings given the string and its SA and LCP array. 
- `src_crochemore/` - Source code for computing the MRC array with no pre-processing of the string.  
- `strings/` - Contains `smallStringArrayGenerator` tool and sample text files for generating suffix arrays and LCP arrays for small substrings.

## Installation and Compilation

### Compiling and executing the Small String Array Generator

1. **Navigate to the `strings/` directory**:
    ```bash
    cd strings
    ```

2. **Compile** the `smallStringArrayGenerator`:
    ```bash
    make
    ```

3. **Run the generator** to create suffix and LCP arrays for a given string. Below is an example of usage:

    ```bash
    ./smallStringArrayGenerator
    ```

    - **Example Input**:
      - Text: `abacaba`
      - Filename: `t1`

    - **Expected Output Files**:
      - `t1` - Original text file with the string.
      - `t1.sa` - Binary file containing the Suffix Array.
      - `t1.lcp` - Binary file containing the LCP Array.

### Compiling and Executing the closedStrings program

Once you have generated the necessary input files, you can proceed to compile and execute the closedStrings program in the `src` folder.

1. **Navigate to the `src/` directory**:
    ```bash
    cd src
    ```

2. **Compile** the closedStrings program:
    ```bash
    make
    ```

    This will generate an executable named `closedStrings`.

3. **Run the closedStrings program**:
    ```bash
    ./closedStrings <text_file_without_extension> [cs|mcs]
    ```

- Input File Format
The input files should follow this format:
    - `<file_name>` - Text file containing a sequence of 1-byte characters.
    - `<file_name>.sa` - Binary file representing the Suffix Array, with each element as a 4-byte integer.
    - `<file_name>.lcp` - Binary file representing the Longest Common Prefix (LCP) Array, with each element as a 4-byte integer. The LCP[0] = 0 is assumed. 
    - **Note:** all these files are assumed to be in the same directory. 

- Input Parameters
    - Replace `<text_file_without_extension>` with the base name of your files.
    - **`cs`** — Computes a **compact representation of all closed strings** of $w[1..n]$.
    - **`mcs`** — Computes **all maximal closed strings** of $w[1..n]$.
    - **Note:** Both options will also compute the **MRC array**, which contains auxiliary data relevant to the closed string computations.

## Examples

### Small String Generation Example
    
    vscode ➜ /workspaces/closedSubstrings/strings (main) $ make
    g++ -std=c++17 -O3 -o smallStringArrayGenerator smallStringArrayGenerator.cpp
    vscode ➜ /workspaces/closedSubstrings/strings (main) $ ./smallStringArrayGenerator
    Enter the string to compute SA and LCP: mississippi
    Enter the base filename: t4
    String, SA, and LCP array written to t4, t4.sa, and t4.lcp

- Note that the `smallStringArrayGenerator` executable only computes SA and LCP for strings that have a length < 10000. 

### Compact Representation of all Closed Strings 

    vscode ➜ /workspaces/closedSubstrings/strings (main) $ ../src/closedStrings t4 cs
    The MRC Array for the string mississippi : 
    0: (1,0), 
    1: (7,4), (1,0), 
    2: (6,3), (2,1), 
    3: (5,2), (3,1), (1,0), 
    4: (4,1), (1,0), 
    5: (2,1), 
    6: (1,0), 
    7: (4,1), (1,0), 
    8: (2,1), 
    9: (1,0), 
    10: (1,0), 
    The Compact Representation of all Closed Strings of string mississippi : 
    m : (0, 1, 1)
    ississi : (1, 4, 7)
    i : (1, 1, 1)
    ssissi : (2, 5, 6)
    ss : (2, 1, 2)
    sissi : (3, 5, 5)
    sis : (3, 3, 3)
    s : (3, 1, 1)
    issi : (4, 4, 4)
    i : (4, 1, 1)
    ss : (5, 1, 2)
    s : (6, 1, 1)
    ippi : (7, 4, 4)
    i : (7, 1, 1)
    pp : (8, 1, 2)
    p : (9, 1, 1)
    i : (10, 1, 1)

### All Maximal Closed Substrings

    vscode ➜ /workspaces/closedSubstrings/strings (main) $ ../src/closedStrings t4 mcs
    The MRC Array for the string mississippi : 
    0: (1,0), 
    1: (7,4), (1,0), 
    2: (6,3), (2,1), 
    3: (5,2), (3,1), (1,0), 
    4: (4,1), (1,0), 
    5: (2,1), 
    6: (1,0), 
    7: (4,1), (1,0), 
    8: (2,1), 
    9: (1,0), 
    10: (1,0), 
    MCS List for the string mississippi : 
    m, ississi, i, ss, sis, i, ss, ippi, i, pp, i,

### Compiling and Executing the closedStringsCrochemore program

Once you have generated the necessary input files, you can proceed to compile and execute the closedStrings program in the `src` folder.

1. **Navigate to the `src_crochemore/` directory**:
    ```bash
    cd src_crochemore
    ```

2. **Compile** the closedStrings program:
    ```bash
    make
    ```

    This will generate an executable named `closedStringsCrochemore`.

3. **Run the closedStringsCrochemore program**:
    ```bash
    ./closedStringsCrochemore <text_file_without_extension>
    ```

- Input File Format
The input file should follow this format:
    - `<file_name>` - Text file containing a sequence of 1-byte characters.
    - **Note:** this file is assumed to be in the same directory. 

## Examples

### Small String Generation Example
    
    vscode ➜ /workspaces/closedSubstrings/strings (main) $ make
    g++ -std=c++17 -O3 -o smallStringArrayGenerator smallStringArrayGenerator.cpp
    vscode ➜ /workspaces/closedSubstrings/strings (main) $ ./smallStringArrayGenerator
    Enter the string to compute SA and LCP: mississippi
    Enter the base filename: t4
    String, SA, and LCP array written to t4, t4.sa, and t4.lcp

- Note that the `smallStringArrayGenerator` executable only computes SA and LCP for strings that have a length < 10000. 

### The MRC Array with no Pre-processing of the given String
    vscode ➜ /workspaces/closedStrings/strings (TOCS_2026) $ ../src_crochemore/closedStringsCrochemore t4
    The MRC Array for the string MISSISSIPPI : 
    0: (1,0), 
    1: (7,4), (1,0), 
    2: (6,3), (2,1), 
    3: (5,2), (3,1), (1,0), 
    4: (4,1), (1,0), 
    5: (2,1), 
    6: (1,0), 
    7: (4,1), (1,0), 
    8: (2,1), 
    9: (1,0), 
    10: (1,0), 

### Large Strings 
- For large strings please generate required input files using efficient algorithms to compute the SA and LCP arrays in the above specified format. The binary read and write snippet can be used from the `./strings/smallStringArrayGenerator.cpp` and the `./src/closedStrings.cpp`. 

## Requirements
To compile and run this project, ensure that the following tools and environment are set up:
1. **Operating System**: 
   - This project is compatible with Linux environments.
   - Alternatively, it can be run inside a VS Code devcontainer for cross-platform support.
2. **Compiler**:
   - Requires `GNU g++` with support for C++17 standard (`std=c++17`).
3. **Build Tool**:
   - `make` is required to handle the compilation process.

