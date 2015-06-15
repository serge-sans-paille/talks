#include <iostream>
#include <sstream>

class Capture {
    // {{{
  std::ostringstream _capture;
  std::streambuf *_old;

  public:
  Capture() {
    _old = std::cout.rdbuf();
    std::cout.rdbuf(_capture.rdbuf());
  }

  ~Capture() {
    std::cout.rdbuf(_old);
  }

  std::string str() const {
    return _capture.str();
  }
    // }}}
};

void verbose() {
  for(int i=0; i< 10; ++i)
    std::cout << "I like poneys" << std::endl;
}

int main() {
  std::cout << "yeah" << std::endl; ;
  {
    Capture C;
    verbose();
  }
  std::cout << "youpi" << std::endl;
}
// vim: foldmethod=marker
