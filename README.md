# FourInARow
A simple, secure FourInARow client-server application for the Foundations of Cybersecurity project.

# Project Objectives

// TODO

# Project Structure

## Folder
We have 4 folders. Folders purpose is:

- `documentation/` (doxygen or any kind of documentation)
- `include/` (**public** interface for the library *.h)
- `libs/` (source **third party** for the application library *.cpp *.hpp)
- `src/` (**private** source for the application)
- `tmp/` (for quick tests)
- `bin` - **input executables** goes here

**Note**: headers files in include goes under folder named after your library domain. Reason behind this is that when we expose public header files we expose only include directory and when we `#include` files from library, instead, you  do this> `#inlcude <HMM/Algorithm.h>` instead of `#include "Algorithm.h"` if it was in root of include

Concrete example:
```
HMM
 ├── include
 │   └── HMM
 │       └── Algorithm.h
 ├── src
 │   ├── Algorithm.cpp
 │   ├── someMath.h
 │   └── someMath.cpp
 └── test
     ├── AlgorithmTests.cpp
     └── mathTests.cpp
```

## Naming Convention

We adopt the following naming convention:

- Class name: CamelCase
- Class member: CamelCase
- Variable: under_score