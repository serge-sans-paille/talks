#include <iostream>
#include <cstdlib>
#define DUMP(n) do { std::cout << n << std::endl; abort(); } while(0)

int main(int argc, char*argv[]) {
  int a = 1;
  if(argc > 1)
    DUMP(a);
  else
    std::cout << "ok" << std::endl;
  return 0;
}
// vim: foldmethod=marker
