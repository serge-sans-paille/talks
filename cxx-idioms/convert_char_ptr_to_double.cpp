#include <iostream>
int main(int argc, char* argv[]) {
  if(argc != 2) return 1;
  // {{{
  std::cout << std::stod(argv[1]) << std::endl;
  // }}}
  return 0;
}
// vim: foldmethod=marker
