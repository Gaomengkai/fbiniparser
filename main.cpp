#include "inidoc.hpp"
#include <ostream>

int main() {
  // clear screen
  printf("\033[2J");
  printf("\033[0;0H");
  printf("==========================================\n");
  printf("      INI Parser by Lex & Bison\n");
  printf("          Writen by Merky\n");
  printf("==========================================\n");

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
  return 0;
}
