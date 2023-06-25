#ifndef INIDOC_HPP
#define INIDOC_HPP
#include <cstddef>
#include <cstdio>
#include <istream>
#include <memory>
#include <ostream>
// #define YYDEBUG 1

#include "ast.hpp"
#include <iostream>

extern int yyparse();
extern FILE *yyin;
extern std::unique_ptr<AST> root;

namespace INIParser {


class INISection {
private:
  std::string _name;
  std::unordered_map<std::string, std::string> _items;

public:
  INISection(const std::string &name) : _name(name) {}
  INISection(const std::string *name) : _name(*name) {}
  INISection(std::unique_ptr<SectionAST> s) {
    _name = s->name;
    for (auto &item : s->items) {
      _items[item.first] = item.second;
    }
  }
  std::string name() const { return _name; }
  std::string get(const std::string &key) const {
    auto it = _items.find(key);
    if (it != _items.end()) {
      return it->second;
    }
    return "";
  }
  void add(const std::unique_ptr<SectionItemsAST> sis) {
    for (auto &item : sis->items) {
      _items[item.first] = item.second;
    }
  }
  void add(const std::unique_ptr<SectionItemAST> si) {
    _items[std::move(*si->key)] = std::move(*si->value);
  }
  void merge(const std::unique_ptr<INISection> isn) {
    for (auto &item : isn->_items) {
      _items[item.first] = item.second;
    }
  }
  std::string operator[](const std::string &key) const { return get(key); }
  void print() const {
    std::cout << std::flush;
    std::cout << "[" << _name << "]" << std::endl;
    for (auto &item : _items) {
      std::cout << item.first << "=" << item.second << std::endl;
    }
  }

  std::string toString() const {
    std::string str = "[" + _name + "]\n";
    for (auto &item : _items) {
      str += item.first + "=" + item.second + "\n";
    }
    return str;
  }
  class iterator {
  private:
    std::unordered_map<std::string, std::string>::const_iterator _it;

  public:
    iterator(
        const std::unordered_map<std::string, std::string>::const_iterator &it)
        : _it(it) {}
    iterator operator++() { return ++_it; }
    bool operator!=(const iterator &other) const { return _it != other._it; }
    std::pair<std::string, std::string> operator*() const { return *_it; }
  };
  iterator begin() const { return iterator(_items.begin()); }
  iterator end() const { return iterator(_items.end()); }
};

class INIDocument {
private:
  std::unordered_map<std::string, std::unique_ptr<INISection>> _sections;

public:
  INIDocument() {}
  INIDocument(std::unique_ptr<AST> ast) {
    for (auto &section : ast->sections) {
      auto is = new INISection(std::move(section.second));
      _sections[section.first] = std::unique_ptr<INISection>(is);
    }
  }
  INISection operator[](const std::string &name) const {
    auto it = _sections.find(name);
    if (it != _sections.end()) {
      return *it->second;
    }
    throw std::out_of_range("No such section");
  }
  void add(std::unique_ptr<INISection> isn) {
    auto name = isn->name();
    auto it = _sections.find(name);
    if (it != _sections.end()) {
      it->second->merge(std::move(isn));
    } else {
      _sections[name] = std::move(isn);
    }
  }
  virtual void print() {
    for (auto &section : _sections) {
      section.second->print();
    }
  }
  std::string toString() const {
    std::string str;
    for (auto &section : _sections) {
      str += section.second->toString();
    }
    return str;
  }
};

class INIFile {
private:
  FILE *_fp;

public:
  INIFile(const char *filename) {
    _fp = fopen(filename, "r");
    if (_fp == nullptr) {
      throw std::runtime_error("Cannot open file");
    }
  }
  INIDocument parse() {
    yyin = _fp;
    yyparse();
    fclose(yyin);
    _fp = nullptr;
    return INIDocument(std::move(root));
  }
  ~INIFile() {
    if (_fp != nullptr) {
      fclose(_fp);
      _fp = nullptr;
    }
  }
};
} // namespace INIParser
#endif // INIDOC_HPP