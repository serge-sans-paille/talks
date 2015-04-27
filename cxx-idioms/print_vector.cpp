#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
int main() {
  std::vector<int> V{{1,2,3}};
  // {{{
  std::accumulate(V.begin() + 1, V.end(),
                  std::ref(std::cout << V.front()),
                  [](std::ostream& os, int v) -> std::ostream& { return os << ", " << v; }
      );
  // }}}
  return 0;
};

// vim: foldmethod=marker
