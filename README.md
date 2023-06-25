# INI PARSER WRITTEN IN FLEX AND BISON

======================================


## Introduction

This is a simple ini parser written in flex and bison. It is a simple example of how to use flex and bison to parse a simple language. The language is a subset of the ini file format. The parser is able to parse the following ini file:

```cpp
// #include "inidoc.hpp"
  try {
    auto iniFile = INIParser::INIFile("demo.ini");
    auto doc = iniFile.parse();

    auto sec = doc["section1"];
    sec.print();

    auto ip = sec["ip"];
    std::cout << ip << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
  }
```

## Usage

- `make` to build the parser
- `make clean` to clean the build files
- `mkdir build && cd build && cmake .. && make` to build the parser with cmake