# FourInARow
A simple, secure FourInARow client-server application for the Foundations of Cybersecurity project.

# Project Objectives

// TODO

# Project Structure

We have 4 folders. Folders purpose is:

- `bin` - **tput executables** goes here
- `documentation` - This folder contains **project documentation**
- `include` - **public** header file (.h files)
- `libs` third party libraries we depend on 
- `src` - **private** source files (.h and .cpp files)
- `test` - **tests** files if has been written

**Note**: Headers files in include are under folder named after our library domain . The reason behind this strategy is that in this way when we expose public headers files, we expose only include directory and when we literally `#include` files from library we're going to do this: `#include <RAND/Algorithm.h>` instead of `#include "Algorithm.h"` if it was in root of `include`.
