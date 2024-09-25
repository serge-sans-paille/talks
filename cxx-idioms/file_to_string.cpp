#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

int main() {
  std::ifstream ifs("file_to_string.cpp");
  if(ifs) {
    std::string content{
      // {{{
      std::istreambuf_iterator<char>(ifs),
      std::istreambuf_iterator<char>()
      // }}}
    };
    std::cout << content;
    return 0;
  }
  return 1;
}
// vim: foldmethod=marker
