#ifndef AST_HPP
#define AST_HPP
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>


using StringDict = std::unordered_map<std::string, std::string>;

// 每个kv对
struct SectionItemAST {
    std::unique_ptr<std::string> key;
    std::unique_ptr<std::string> value;
    SectionItemAST(const std::string* kvpair) {
      auto moved = std::make_unique<std::string>(*kvpair);
      auto pos = moved->find('=');
      key = std::make_unique<std::string>(moved->substr(0, pos));
      value = std::make_unique<std::string>(moved->substr(pos + 1));
      // key.strip() in python likes the following code in C++
      key->erase(0, key->find_first_not_of(" "));
      key->erase(key->find_last_not_of(" ") + 1);
      value->erase(0, value->find_first_not_of(" "));
      value->erase(value->find_last_not_of(" ") + 1);
    }

};

struct SectionItemsAST {
    StringDict items;
    void add(const std::unique_ptr<SectionItemAST> si) {
        items[std::move(*si->key)] = std::move(*si->value);
    }
    SectionItemsAST() {
    }
};
class IPrintable {
  virtual void print() const = 0;
};
// 节
class SectionAST :IPrintable{
public:
  SectionAST(const std::string &name) : name(name) {}
  SectionAST(const std::string* name) : name(*name) {}
  std::string name;
  std::unordered_map<std::string, std::string> items;
  std::string get(const std::string &key);
  void add(const std::unique_ptr<SectionItemsAST> sis) {
    for (auto &item : sis->items) {
      items[item.first] = item.second;
    }
  }
  void print() const{
    for (auto &item : items) {
      std::cout<<"ITEM > FIRST = "<<item.first<<"\n       SECOND = "<<item.second<<std::endl;
      std::cout<<item.first[0]<<item.first.length()<<std::endl;
      std::cout<<item.second[0]<<item.second.length()<<std::endl;
    }
  }

};

class SectionListAST {
public:
  std::unordered_map<std::string, std::unique_ptr<SectionAST>> sections;
  void add(std::unique_ptr<SectionAST> section) {
    sections[section->name] = std::move(section);
  }
  SectionListAST() {}

};

// 整个文档
class AST :IPrintable{
public:
  std::unordered_map<std::string, std::unique_ptr<SectionAST>> sections;
  void add(std::unique_ptr<SectionAST> section);
  void add(std::unique_ptr<SectionListAST> sl) {
    for (auto &section : sl->sections) {
      sections[section.first] = std::move(section.second);
    }
  }
  void print() const{
    for (auto &section : sections) {
      std::cout << "SECTION:[" << section.first << "]" << std::endl;
      section.second->print();
    }
  }
};
#endif // AST_HPP
