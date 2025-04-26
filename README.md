name: City-Building Simulator
description: A modular city-building simulator in C++ using OOP principles and Rule of 5 memory management.
features:
  - Modular city-building simulation
  - Facilities, settlements, and planning
  - Dynamic actions and policies
  - Manual memory management (deep copy, move semantics)
technologies:
  - C++
  - Makefile
  - STL
structure: |
  .
  ├── makefile
  ├── bin/
  ├── include/
  ├── src/
  ├── .devcontainer/
  ├── .vscode/
  └── README.md
build: |
  make
run: |
  ./bin/Simulator
notes:
  - Developed using VSCode + Docker devcontainers
  - Written with modern C++ practices
