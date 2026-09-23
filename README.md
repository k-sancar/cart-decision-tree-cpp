# CART Decision Tree Classifier (C++)

## Purpose
This project was developed to understand the core mechanics and mathematics of machine learning algorithms. Instead of using high-level libraries, I implemented a binary CART decision tree classifier from scratch. 

The program operates as a medical diagnosis assistant that analyzes patient symptom data to predict health conditions, based on the dedicated SQL table.

## Key Goals Achieved
* **Understanding Core ML Mechanics:** Implemented recursive tree-building and node-splitting logic using Gini Impurity.
* **SQLite Integration:** Connected the classifier directly to an SQLite database using `sqlite_orm` to manage training data and patient records.
* **Export/Import:** Created a system to export the trained model to a CSV file and load it back into memory for predictions.

## Compilation & Execution

### Prerequisites
Ensure you have the GCC compiler collection (`gcc` and `g++`) installed and accessible via your system's PATH.

### Build Instructions (Windows / PowerShell)

**1. Compile the SQLite C library into an object file:**
```bash
gcc -c external/sqlite3.c -o external/sqlite3.o
```

**2. Compile the C++ source files and link them with the SQLite object:**
```bash
g++ -std=c++17 src/decision_tree.cpp src/diagnosis.cpp src/main.cpp src/node.cpp src/question.cpp src/tree_exporter.cpp src/tree_importer.cpp external/sqlite3.o -Iexternal -o program.exe
```

### Running the Application
Once compiled successfully, launch the interactive diagnostic menu directly from the root directory to ensure correct database path resolution:
```bash
./program.exe
```