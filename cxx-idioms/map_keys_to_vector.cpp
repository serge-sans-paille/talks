#include<iostream>
#include<map>
#include<vector>
#include<algorithm>

int main() {
  std::map<int, float> const map{{0,1.},{1,3.}, {2, 9.}};

  std::vector<int> keys;
  keys.reserve(map.size());
  std::transform(map.begin(), map.end(),
                 std::back_inserter(keys),
                 [](auto&& kv) { return kv.first; }
      );
  std::cout << keys[0] << ' ' << keys[1] << ' ' << keys[2] << std::endl;
  return 0;
}
